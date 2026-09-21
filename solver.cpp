#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum Cat
{
  satisfied,
  unsatisfied,
  normal,
  completed
};

class Formula
{
public:
  int num_vars;
  vector<int> literals;
  vector<int> literal_frequency;
  vector<int> literal_polarity;
  vector<vector<int>> clauses;

  Formula()
  {
    num_vars = 0;
  }

  Formula(const Formula &f)
  {
    num_vars = f.num_vars;
    literals = f.literals;
    clauses = f.clauses;
    literal_frequency = f.literal_frequency;
    literal_polarity = f.literal_polarity;
  }

  void recalculate_stats()
  {
    literal_frequency.assign(num_vars, 0);
    literal_polarity.assign(num_vars, 0);
    literals.assign(num_vars, -1);

    for (int i = 0; i < clauses.size(); i++)
    {
      for (int j = 0; j < clauses[i].size(); j++)
      {
        int lit = clauses[i][j];
        int var = lit / 2;

        literal_frequency[var]++;

        if (lit % 2 == 0)
        {
          literal_polarity[var]++;
        }
        else
        {
          literal_polarity[var]--;
        }
      }
    }
  }
};

Formula readCNF(string filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    cout << "Erro ao abrir: " << filename << endl;
    exit(1);
  }

  Formula formula;
  char c;
  string s;
  int clause_count;

  while (file >> c)
  {
    if (c == 'c')
    {
      getline(file, s);
    }
    else if (c == 'p')
    {
      file >> s;
      file >> formula.num_vars;
      file >> clause_count;
      break;
    }
  }

  formula.clauses.resize(clause_count);
  int literal;

  for (int i = 0; i < clause_count; i++)
  {
    while (file >> literal && literal != 0)
    {
      if (literal > 0)
      {
        formula.clauses[i].push_back(2 * (literal - 1));
      }
      else
      {
        formula.clauses[i].push_back(2 * ((-1) * literal - 1) + 1);
      }
    }
  }

  file.close();
  formula.recalculate_stats();
  return formula;
}

class SATSolverDPLL
{
private:
  int unit_propagate(Formula &f);
  int DPLL(Formula f);
  int apply_transform(Formula &f, int literal_to_apply);

public:
  bool is_sat(Formula f);
};

int SATSolverDPLL::unit_propagate(Formula &f)
{
  bool unit_clause_found;
  if (f.clauses.size() == 0)
  {
    return Cat::satisfied;
  }

  do
  {
    unit_clause_found = false;
    for (int i = 0; i < f.clauses.size(); i++)
    {
      if (f.clauses[i].size() == 1)
      {
        unit_clause_found = true;
        int lit = f.clauses[i][0];
        f.literals[lit / 2] = lit % 2;
        f.literal_frequency[lit / 2] = -1;

        int result = apply_transform(f, lit / 2);
        if (result == Cat::satisfied || result == Cat::unsatisfied)
        {
          return result;
        }
        break;
      }
      else if (f.clauses[i].size() == 0)
      {
        return Cat::unsatisfied;
      }
    }
  } while (unit_clause_found);

  return Cat::normal;
}

int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply)
{
  int value_to_apply = f.literals[literal_to_apply];

  for (int i = 0; i < f.clauses.size(); i++)
  {
    for (int j = 0; j < f.clauses[i].size(); j++)
    {
      if ((2 * literal_to_apply + value_to_apply) == f.clauses[i][j])
      {
        f.clauses.erase(f.clauses.begin() + i);
        i--;
        if (f.clauses.size() == 0)
        {
          return Cat::satisfied;
        }
        break;
      }
      else if (f.clauses[i][j] / 2 == literal_to_apply)
      {
        f.clauses[i].erase(f.clauses[i].begin() + j);
        j--;
        if (f.clauses[i].size() == 0)
        {
          return Cat::unsatisfied;
        }
        break;
      }
    }
  }
  return Cat::normal;
}

int SATSolverDPLL::DPLL(Formula f)
{
  int result = unit_propagate(f);
  if (result == Cat::satisfied)
    return Cat::satisfied;
  if (result == Cat::unsatisfied)
    return Cat::unsatisfied;

  int max_freq = -1;
  int variavel_escolhida = 0;

  for (int k = 0; k < f.literal_frequency.size(); k++)
  {
    if (f.literal_frequency[k] > max_freq)
    {
      max_freq = f.literal_frequency[k];
      variavel_escolhida = k;
    }
  }

  for (int j = 0; j < 2; j++)
  {
    Formula new_f = f;

    if (new_f.literal_polarity[variavel_escolhida] > 0)
    {
      new_f.literals[variavel_escolhida] = j;
    }
    else
    {
      new_f.literals[variavel_escolhida] = (j + 1) % 2;
    }

    new_f.literal_frequency[variavel_escolhida] = -1;

    int transform_result = apply_transform(new_f, variavel_escolhida);
    if (transform_result == Cat::satisfied)
      return Cat::satisfied;
    if (transform_result == Cat::unsatisfied)
      continue;

    if (DPLL(new_f) == Cat::satisfied)
      return Cat::satisfied;
  }

  return Cat::unsatisfied;
}

bool SATSolverDPLL::is_sat(Formula f)
{
  return DPLL(f) == Cat::satisfied;
}

bool check_difference(Formula F_base, vector<int> C_alvo, SATSolverDPLL &solver)
{
  for (int i = 0; i < C_alvo.size(); i++)
  {
    int lit = C_alvo[i];
    int negated_lit;

    if (lit % 2 == 0)
    {
      negated_lit = lit + 1;
    }
    else
    {
      negated_lit = lit - 1;
    }

    vector<int> clausula_unitaria;
    clausula_unitaria.push_back(negated_lit);
    F_base.clauses.push_back(clausula_unitaria);
  }

  F_base.recalculate_stats();
  return solver.is_sat(F_base);
}

bool check_equivalence(Formula f1, Formula f2, SATSolverDPLL &solver)
{

  int max_vars = f1.num_vars;
  if (f2.num_vars > max_vars)
  {
    max_vars = f2.num_vars;
  }
  f1.num_vars = max_vars;
  f2.num_vars = max_vars;

  // Testa F1 contra F2
  for (int i = 0; i < f2.clauses.size(); i++)
  {
    if (check_difference(f1, f2.clauses[i], solver) == true)
    {
      return false;
    }
  }

  // Testa F2 contra F1
  for (int i = 0; i < f1.clauses.size(); i++)
  {
    if (check_difference(f2, f1.clauses[i], solver) == true)
    {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[])
{
  vector<string> argumentos;
  for (int i = 0; i < argc; i++)
  {
    argumentos.push_back(argv[i]);
  }

  string file1 = argumentos[1];
  string file2 = argumentos[2];

  Formula f1 = readCNF(file1);
  Formula f2 = readCNF(file2);

  SATSolverDPLL solver;

  if (check_equivalence(f1, f2, solver) == true)
  {
    cout << "Iguais (Equivalentes)" << endl;
  }
  else
  {
    cout << "Diferentes" << endl;
  }

  return 0;
}