#define OUT__ 0
#define RESEARCH__ 1
#define FUNCTION__ 2
#define NOMINMAX
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
using namespace std;
wstring Text;
vector<wstring> Textlines;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

size_t Debug(size_t i, size_t left, size_t right)
{
	system("cls");
	for (size_t j = i - left; j < i; ++j)
	{
		wcout << Textlines[j] << L'\n';
	}
	SetConsoleTextAttribute(hConsole, 4);
	wcout << Textlines[i] << L'\n';
	SetConsoleTextAttribute(hConsole, 15);
	for (size_t j = i + 1; j <= i + right; ++j)
	{
		wcout << Textlines[j] << L'\n';
	}
	return left + right;
}

size_t debug(size_t i, size_t left, size_t right)
{
	system("cls");
	wcout << Text.substr(i - left, left);
	SetConsoleTextAttribute(hConsole, 64);
	wcout << Text.at(i);
	SetConsoleTextAttribute(hConsole, 15);
	wcout << Text.substr(i + 1, right);
	return left + right;
}

int main()
{
	SetConsoleTextAttribute(hConsole, 15);

	// variabili
	wifstream file("inputfile.txt");
	wstring line;

	// input
	if (!file.is_open())
	{
		wcerr << L"Errore nell'apertura del file per la lettura.\n";
		return 0;
	}
	while (getline(file, line))
	{
		Textlines.push_back(line);
	}
	file.close();

	// rimozione spazzatura varia
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		if (i == 11067)
		{
			Debug(i, 10, 10);
			i = i;
		}

		// riga vuota
		if (Textlines[i].empty())
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}

		// rimozione spazi bianchi a sinistra
		bool ContinueLoop{ false };
		int spaces{};
		for (; Textlines[i].at(spaces) == L' ' or Textlines[i].at(spaces) == L'\t';
			++spaces)
		{
			if (spaces == Textlines[i].size() - 1)
			{
				Textlines.erase(Textlines.begin() + i);
				break;
			}
		}
		if (ContinueLoop)
		{
			continue;
		}
		Textlines[i].erase(0, spaces);

		// riga corta
		if (Textlines[i].empty())
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].size() == 1)
		{
			continue;
		}

		// riga commentata
		if (Textlines[i].at(0) == L'/' and Textlines[i].at(1) == L'/')
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}

		// riga interrotta
		if (i == Textlines.size() - 1)
		{
			continue;
		}
		if (Textlines[i].at(Textlines[i].size() - 1) == L'\\' and
			Textlines[i].at(Textlines[i].size() - 2) != L'\\')
		{
			Textlines[i].pop_back();
			Textlines[i] += Textlines[i + 1];
			Textlines.erase(Textlines.begin() + i + 1);
		}
	}

	// rimozione di tutti i commenti
	bool remove{ false };
	vector<size_t> i1{}, i2{}, j1{}, j2{};
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		int quotation_marks{}, apostrophes{};
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			switch (Textlines[i].at(j))
			{
			case L'\"': {
				bool escaped{ false };
				if (j > 0)
				{
					int k = 1;
					while (Textlines[i].at(j - k) == L'\\')
					{
						escaped = !escaped;
						k++;
						if (j < k)
						{
							break;
						}
					}
				}
				if (escaped)
				{
					break;
				}
				if (!remove)
				{
					quotation_marks++;
				}
				break;
			}

			case L'\'': {
				bool escaped{ false };
				if (j > 0)
				{
					int k = 1;
					while (Textlines[i].at(j - k) == L'\\')
					{
						escaped = !escaped;
						k++;
						if (j < k)
						{
							break;
						}
					}
				}
				if (escaped)
				{
					break;
				}
				if (!remove)
				{
					apostrophes++;
				}
				break;
			}

			case L'/':
				if (remove)
				{
					break;
				}
				if (j + 1 >= Textlines[i].size())
				{
					break;
				}
				if (quotation_marks % 2 != 0 or apostrophes % 2 != 0)
				{
					break;
				}

				// commento semplice
				if (Textlines[i].at(j + 1) == L'/')
				{
					Textlines[i].erase(j);
					break;
				}

				// commento multilinea
				if (Textlines[i].at(j + 1) == L'*')
				{
					remove = true;
					i1.push_back(i);
					j1.push_back(j);
					break;
				}
				break;

				// fine eliminazione
			case L'*':
				if (!remove)
				{
					break;
				}

				if (j + 1 >= Textlines[i].size())
				{
					break;
				}

				if (Textlines[i].at(j + 1) == L'/')
				{
					remove = false;
					i2.push_back(i);
					j2.push_back(j);
					break;
				}
				break;
			}
		}
	}
	for (size_t i = 0; i < i1.size(); ++i)
	{
		if (i2[i] == i1[i])
		{
			Textlines[i1[i]].erase(j1[i], j2[i] - j1[i] + 2);
			continue;
		}

		Textlines[i1[i]].erase(j1[i]);
		Textlines[i2[i]].erase(0, j2[i] + 1);
		for (size_t j = i1[i] + 1; j < i2[i]; ++j)
		{
			Textlines.erase(Textlines.begin() + i1[i]);
		}
	}

	// rimozione testo
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		vector<long long> indexes;
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			switch (Textlines[i].at(j))
			{
			case L'\"': {
				bool escaped{ false };
				if (j > 0)
				{
					int k = 1;
					while (Textlines[i].at(j - k) == L'\\')
					{
						escaped = !escaped;
						k++;
						if (j < k)
						{
							break;
						}
					}
				}
				if (escaped)
				{
					break;
				}
				indexes.push_back(j);
			}}
		}
		for (long long j = indexes.size() - 1; j >= 0; j -= 2)
		{
			Textlines[i].erase(indexes[j - 1], indexes[j] - indexes[j - 1] + 1);
		}

		// rimozione apostrofi
		indexes.clear();
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			switch (Textlines[i].at(j))
			{
			case L'\'': {
				bool escaped{ false };
				if (j > 0)
				{
					int k = 1;
					while (Textlines[i].at(j - k) == L'\\')
					{
						escaped = !escaped;
						k++;
						if (j < k)
						{
							break;
						}
					}
				}
				if (escaped)
				{
					break;
				}
				if (j > 0 and j < Textlines[i].size() - 1)
				{
					if (isdigit(Textlines[i].at(j - 1)) and
						isdigit(Textlines[i].at(j + 1)))
					{
						break;
					}
				}
				indexes.push_back(j);
			}}
		}
		for (long long j = indexes.size() - 1; j >= 0; j -= 2)
		{
			Textlines[i].erase(indexes[j - 1], indexes[j] - indexes[j - 1] + 1);
		}
	}

	// rimozione di alcune direttive
	vector<wstring> MacroNames, MacroValues, macros;
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		// aggiunta macro alla lista
		if (Textlines[i].find(L"#define") != wstring::npos)
		{
			// rimozione spazi bianchi a sinistra
			Textlines[i].erase(0, 7);
			bool ContinueLoop{ false };
			int spaces{};
			for (; Textlines[i].at(spaces) == L' ' or
				Textlines[i].at(spaces) == L'\t'; ++spaces)
			{
				if (spaces == Textlines[i].size() - 1)
				{
					Textlines.erase(Textlines.begin() + i);
					ContinueLoop = true;
					break;
				}
			}
			if (ContinueLoop)
			{
				continue;
			}
			Textlines[i].erase(0, spaces);

			// calcolo fine del nome
			bool activated{ true };
			size_t Firstspace{ Textlines[i].size() - 1 };
			for (size_t j = 0; j < Textlines[i].size(); ++j)
			{
				if (Textlines[i].at(j) == L'(')
				{
					activated = false;
					continue;
				}
				if (Textlines[i].at(j) == L')')
				{
					activated = true;
					continue;
				}
				if ((Textlines[i].at(j) == L' '
					or Textlines[i].at(j) == L'\t') and activated)
				{
					Firstspace = j;
					break;
				}
			}

			// separazione nome e valore
			if (Firstspace != wstring::npos)
			{
				MacroNames.push_back(Textlines[i].substr(0, Firstspace));
				MacroValues.push_back(Textlines[i].substr(Firstspace + 1));
			}
			else
			{
				macros.push_back(Textlines[i]);
			}
			Textlines.erase(Textlines.begin() + i);
			continue;
		}

		// rimozione direttive inutili
		if (Textlines[i].find(L"#include") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].find(L"#ifdef") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].find(L"#ifndef") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].find(L"#else") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].find(L"#endif") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
		if (Textlines[i].find(L"#pragma") != wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
			continue;
		}
	}

	// unione righe
	for (const auto& text : Textlines)
	{
		Text += text + L'\n';
	}

	// sostituzione delle macro semplici
	for (const auto& macro : macros)
	{
		size_t pos{};
		while ((pos = Text.find(macro, pos)) != wstring::npos)
		{
			Text.erase(pos, macro.size());
		}
	}

	// separazione argomenti 
	vector<vector<wstring>> args(MacroNames.size());
	for (size_t i = 0; i < MacroNames.size(); ++i)
	{
		auto pos{ MacroNames[i].find(L'(') };
		if (pos == wstring::npos)
		{
			continue;
		}
		auto name{
			MacroNames[i].substr(pos + 1, MacroNames[i].find(L')') - pos - 1)
		};

		size_t first{}, last{};
		for (size_t j = 0; j < name.size(); ++j)
		{
			if (isalpha(name.at(j)) or isdigit(name.at(j)) or name.at(j) == L'_')
			{
				last++;
				continue;
			}

			auto push{ name.substr(first, last - first) };
			if (!push.empty())
			{
				args[i].push_back(push);
			}
			first = ++last;
		}
		auto push{ name.substr(first, last - first) };
		if (!push.empty())
		{
			args[i].push_back(push);
		}
		MacroNames[i].erase(pos);
	}

	// riorganizzazione valori
	vector<vector<size_t>> indeces(MacroValues.size());
	vector<vector<wstring>> concats;
	for (size_t i = 0; i < MacroValues.size(); ++i)
	{
		concats.push_back({ MacroValues[i] });
		for (size_t j = 0; j < args[i].size(); ++j)
		{
			bool finished{ true };
			for (size_t k = 0; k < concats[i].size(); ++k)
			{
				size_t pos{};
				if ((pos = concats[i][k].find(args[i][j], pos)) != wstring::npos)
				{
					finished = false;
					concats[i][k].erase(pos, args[i][j].size());
					concats[i].insert(
						concats[i].begin() + k + 1, concats[i][k].substr(pos)
					);
					concats[i][k].erase(pos);
					indeces[i].insert(indeces[i].begin() + k, j);
					break;
				}
			}

			if (!finished)
			{
				j--;
			}
		}
	}

	// sostituzione di tutte le macro
	for (size_t i = 0; i < MacroNames.size(); ++i)
	{
		vector<wstring> params;
		size_t pos{};

		// ricerca
		while ((pos = Text.find(MacroNames[i], pos)) != wstring::npos)
		{
			if (MacroNames[i] != Text.substr(pos, MacroNames[i].size()))
			{
				pos += MacroNames[i].size();
				continue;
			}

			// macro senza argomenti
			if (args[i].empty())
			{
				Text.erase(pos, MacroNames[i].size());
				Text.insert(pos, concats[i][0]);
				pos += concats[i][0].size();
				continue;
			}

			// ricerca parte di testo contenente le informazioni
			auto backupPos{ pos };
			pos += MacroNames[i].size();
			auto firstpos{ pos };
			int ParenthesisBalance{};
			for (; pos < Text.size(); ++pos)
			{
				switch (Text.at(pos))
				{
				case L'(':
					ParenthesisBalance++;
					break;
				case L')':
					ParenthesisBalance--;
					break;
				}
				if (ParenthesisBalance == 0)
				{
					break;
				}
			}

			// estrazione informazioni
			auto parameters{ Text.substr(firstpos + 1, pos - firstpos - 1) };
			firstpos = 0;
			for (size_t j = 0; j < parameters.size(); ++j)
			{
				switch (Text.at(pos))
				{
				case L'(':
					ParenthesisBalance++;
					break;
				case L')':
					ParenthesisBalance--;
					break;
				case L',':
					if (ParenthesisBalance != 0)
					{
						break;
					}

					params.push_back(parameters.substr(firstpos, j - firstpos));
					firstpos = j + 1;
				}
			}
			params.push_back(parameters.substr(firstpos));

			// macro falsa
			if (params.size() != args[i].size())
			{
				continue;
			}

			// costruzione testo da sostiuire
			wstring TextToReplace;
			for (size_t j = 0; j < indeces[i].size(); ++j)
			{
				TextToReplace += concats[i][j];
				TextToReplace += params[indeces[i][j]];
			}
			TextToReplace += concats[i].back();

			// sostituzione
			Text.erase(backupPos, MacroNames[i].size() + parameters.size() + 2);
			Text.insert(backupPos, TextToReplace);
			backupPos += TextToReplace.size();
			pos = backupPos;
		}
	}

	// rimozione di altri spazi bianchi
	Text = L"\n" + Text;
	size_t LastNotOf{ Text.size() - 1 };
	for (long long i = LastNotOf; i >= 0; --i)
	{
		if (Text.at(i) == L'\n')
		{
			size_t pos{};
			if ((pos = Text.substr(i, LastNotOf).find(L"__pragma", pos))
				!= wstring::npos)
			{
				Text.erase(i + 1, LastNotOf - i - 1);
				LastNotOf = i;
				continue;
			}

			int spaces{};
			for (; Text.at(i - spaces) == L' ' or Text.at(i - spaces) == L'\t';
				--spaces);
			Text.erase(i + 1 - spaces, spaces);
			LastNotOf = i;
		}
	}
	while (Text.at(0) == L' ' or Text.at(0) == L'\t' or Text.at(0) == L'\n')
	{
		Text.erase(0, 1);
	}
	for (long long i = Text.size() - 1; i > 0; --i)
	{
		if ((Text.at(i) == L' ' or Text.at(i) == L'\t')
			and (Text.at(i - 1) == L' ' or Text.at(i - 1) == L'\t'))
		{
			Text.erase(i, 1);
		}
	}

	// unificazione su una riga
	for (long long i = Text.size() - 1; i > 0; --i)
	{
		if (Text.at(i) == L'\n')
		{
			if (Text.at(i - 1) == L' ' or Text.at(i - 1) == L'\t')
			{
				Text.erase(i, 1);
				continue;
			}
			Text.at(i) = L' ';
		}
	}

	// output iniziale
	wofstream output("output.txt", ios::trunc);
	if (!output.is_open())
	{
		wcerr << L"Errore nell'apertura del file per la scrittura.\n";
		return 0;
	}
	output << Text;
	output.close();

	// rimozione del contenuto delle funzioni
	size_t Index;
	int location{ OUT__ };
	int tabs{}, saved{}, balance{};

	////////////////////////////////////////////////////////////////////////////////
	size_t u{};
	bool re_id{ false }, slide{ false };
re_id_loop:
	if (re_id) goto end_loop;
	////////////////////////////////////////////////////////////////////////////////

	for (size_t i = 0; i < Text.size(); ++i)
	{
		////////////////////////////////////////////////////////////////////////////
		if (slide) debug(i, min(i, size_t(50)), 100);
		if (++u == 185774)
		{
			u = u;
		}
		////////////////////////////////////////////////////////////////////////////

		switch (Text.at(i))
		{
		case L'{':
			if (location == RESEARCH__)
			{
				break;
			}
			if (location == FUNCTION__ and saved == tabs)
			{
				Index = i;
			}
			tabs++;
			break;

		case L'}':
			if (location == RESEARCH__)
			{
				break;
			}
			tabs--;
			if (location == FUNCTION__ and saved == tabs)
			{
				Text.erase(Index + 1, i - Index - 1);
				i = Index;
				location = OUT__;
			}
			break;

		case L'(':
			if (location == FUNCTION__)
			{
				break;
			}
			if (location == OUT__)
			{
				location = RESEARCH__;
				balance = 0;
			}
			balance++;
			break;

		case L')':
			if (location == FUNCTION__)
			{
				break;
			}
			if (location == RESEARCH__)
			{
				balance--;
				if (balance == 0)
				{	
					saved = tabs;
					location = FUNCTION__;
				}
			}
			break;

		case L';':
			if (location == FUNCTION__ and saved == tabs)
			{
				location = OUT__;
			}
			break;
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	re_id = true;
	goto re_id_loop;
end_loop:
	////////////////////////////////////////////////////////////////////////////////

	// output finale
	wofstream final_output("hierarchy.txt", ios::trunc);
	if (!final_output.is_open())
	{
		wcerr << L"Errore nell'apertura del secondo file per la scrittura.\n";
		return 0;
	}
	final_output << Text;
	final_output.close();

	return 0;
}