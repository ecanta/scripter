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

// funzione per visualizzare le righe del testo
size_t Debug(size_t i, size_t left, size_t right, bool off = false)
{
	if (!off)
	{
		system("cls");
	}
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

// funzione per visualizzare il testo
size_t debug(size_t i, size_t left, size_t right, bool off = false)
{
	if (!off)
	{
		system("cls");
	}
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
				ContinueLoop = true;
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
			// conta delle virgolette
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

			// conta degli apostrofi
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
			
			// azioni sui commenti
			case L'/':
				// controlli di sicurezza
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

	// rimozione delle parti segnalate
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
		// rimozione virgolette
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
			}
			}
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
			}
			}
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
			auto Firstspace{ Textlines[i].size() - 1 };
			for (size_t j = 0; j <= Firstspace; ++j)
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
			if (Firstspace != Textlines[i].size() - 1)
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
	for (const auto& name : MacroNames)
	{
		macros.push_back(name);
	}

	// separazione argomenti per le macro definite
	vector<vector<wstring>> args(MacroNames.size());
	for (size_t i = 0; i < MacroNames.size(); ++i)
	{
		// ricerca nomi
		auto pos{ MacroNames[i].find(L'(') };
		if (pos == wstring::npos)
		{
			continue;
		}
		auto name{
			MacroNames[i].substr(pos + 1, MacroNames[i].find(L')') - pos - 1)
		};

		// separazione parole
		size_t first{}, last{};
		for (size_t j = 0; j < name.size(); ++j)
		{
			if (isalnum(name.at(j)) or name.at(j) == L'_')
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

	// riorganizzazione valori per le macro più complesse
	vector<vector<size_t>> indeces(MacroValues.size());
	vector<vector<wstring>> concats;
	for (size_t i = 0; i < MacroValues.size(); ++i)
	{
		concats.push_back({ MacroValues[i] });
		for (size_t j = 0; j < args[i].size(); ++j)
		{
			// sostituzione dei parametri nella definizione
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
	auto LastNotOf{ Text.size() - 1 };
	for (long long i = LastNotOf; i >= 0; --i)
	{
		if (Text.at(i) == L'\n')
		{
			// rimozione dei pragma sfuggiti alle rimozioni precedenti
			size_t pos{};
			if ((pos = Text.substr(i, LastNotOf).find(L"__pragma", pos))
				!= wstring::npos)
			{
				Text.erase(i + 1, LastNotOf - i - 1);
				LastNotOf = i;
				continue;
			}

			// eliminazione spazi aggiuntivi
			int spaces{};
			for (; Text.at(i - spaces) == L' ' or Text.at(i - spaces) == L'\t';
				--spaces);
			Text.erase(i + 1 - spaces, spaces);
			LastNotOf = i;
		}
	}

	// sostituzione delle tabulazioni con spazi semplici
	for (long long i = Text.size() - 1; i > 0; --i)
	{
		if ((Text.at(i) == L' ' or Text.at(i) == L'\t')
			and (Text.at(i - 1) == L' ' or Text.at(i - 1) == L'\t'))
		{
			Text.erase(i, 1);
			if (Text.at(i - 1) == L'\t')
			{
				Text.at(i - 1) = L' ';
			}
		}
	}

	// rimozione di tutti i caratteri di nuova linea
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

	// rimozione della maggiorparte del codice inutile
	bool EraseToCollin{ false };
	size_t CollinIndex, ResearchIndex, Index;
	int location{ OUT__ };
	int tabs{}, saved{}, balance{}, ConstructorBalance{};
	for (size_t i = 0; i < Text.size(); ++i)
	{
		switch (Text.at(i))
		{
		case L'{':
			if (location == RESEARCH__)
			{
				break;
			}

			// indice di inizio rimozione del codice in una funzione
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

			// spostamento fuori dalla funzione
			if (location == FUNCTION__
				and saved == tabs and ConstructorBalance == 0)
			{
				// rimozione codice interno
				Text.erase(Index + 1, i - Index - 1);
				i = Index + 1;
				location = OUT__;

				// rimozione copie nei costruttori in una classe
				if (EraseToCollin)
				{
					size_t FirstNotErased;
					int CollinBalance{ -1 };
					EraseToCollin = false;

					// bilancio delle parentesi per trovare la fine della rimozione
					for (size_t j = i - 1; j > CollinIndex; --j)
					{
						switch (Text.at(j))
						{
						case L'{':
							CollinBalance++;
							break;
						case L'}':
							CollinBalance--;
							break;
						}
						if (CollinBalance == 0)
						{
							FirstNotErased = j;
							break;
						}
					}

					// eliminazione
					Text.erase(CollinIndex, FirstNotErased - CollinIndex);
					i = CollinIndex + 1;
				}
			}
			break;

		case L'(':
			if (location == FUNCTION__)
			{
				ConstructorBalance++;
				break;
			}

			// inizio ricerca parentesi
			if (location == OUT__)
			{
				location = RESEARCH__;
				ResearchIndex = i;
				balance = 0;
			}
			balance++;
			
			break;

		case L')':
			if (location == FUNCTION__)
			{
				ConstructorBalance--;
				break;
			}

			if (location == RESEARCH__)
			{
				balance--;

				// fine ricerca parentesi
				if (balance == 0)
				{
					// determinazione se funzione o se variabile globale
					saved = tabs;
					location = FUNCTION__;
					ConstructorBalance = 0;
					auto params{
						Text.substr(ResearchIndex + 1, i - ResearchIndex - 1)
					};
					
					// funzione senza argomenti
					if (params.empty())
					{
						break;
					}
					if (params == L" ")
					{
						params.clear();
						Text.erase(ResearchIndex + 1);
						i--;
						break;
					}

					// rimozione parentesi tonde aggiuntive
					int ArgBalance{};
					vector<size_t> ArgIndeces;
					for (size_t j = 0; j < params.size(); ++j)
					{
						switch (params.at(j))
						{
						case L'(':
							if (ArgBalance == 0)
							{
								ArgIndeces.push_back(j);
							}
							ArgBalance++;
							break;
						case L')':
							ArgBalance--;
							if (ArgBalance == 0)
							{
								ArgIndeces.push_back(j);
							}
							break;
						}
					}
					for (long long j = ArgIndeces.size() - 1; j >= 0; j -= 2)
					{
						params.erase(
							ArgIndeces[j - 1],
							ArgIndeces[j] - ArgIndeces[j - 1] + 1
						);
					}

					// rimozione parentesi graffe aggiuntive
					int TabBalance{};
					vector<size_t> TabIndeces;
					for (size_t j = 0; j < params.size(); ++j)
					{
						switch (params.at(j))
						{
						case L'{':
							if (TabBalance == 0)
							{
								TabIndeces.push_back(j);
							}
							TabBalance++;
							break;
						case L'}':
							TabBalance--;
							if (TabBalance == 0)
							{
								TabIndeces.push_back(j);
							}
							break;
						}
					}
					for (long long j = TabIndeces.size() - 1; j >= 0; j -= 2)
					{
						params.erase(
							TabIndeces[j - 1],
							TabIndeces[j] - TabIndeces[j - 1] + 1
						);
					}

					// conta delle parole
					bool IsFunction{ params.find(L',') != wstring::npos };
					if (!IsFunction)
					{
						bool ItsAWord{ false }, stopped{ false };
						for (size_t j = 0; j < params.size(); ++j)
						{
							if (params.at(j) == L'_' or
								params.at(j) == L'.' or
								isalnum(params.at(j)))
							{
								if (stopped)
								{
									IsFunction = true;
									break;
								}
								ItsAWord = true;
								continue;
							}
							stopped = true;
						}
					}

					// se è una variabile globale
					if (!IsFunction)
					{
						Text.erase(ResearchIndex, i - ResearchIndex + 1);
						i = ResearchIndex - 1;
						break;
					}

					// rimozione dei segni di uguaglianza
					auto LastComma{ params.size() };
					for (long long j = LastComma - 1; j >= 0; --j)
					{
						switch (params.at(j))
						{
						case L',': LastComma = j;
							break;
						case L'=':
							params.erase(j, LastComma - j);
							while (params.at(--j) == L' ')
							{
								params.erase(j, 1);
							}
							j++;
							break;
						}
					}
					
					// aggiornamento
					Text =
						Text.substr(0, ResearchIndex + 1) + params + Text.substr(i);
					i = ResearchIndex + params.size() + 1;
				}
			}
			break;

			// segnalazione dell'esistenza di un costruttore con copie
		case L':':
			if (location == FUNCTION__
				and saved == tabs and ConstructorBalance == 0)
			{
				EraseToCollin = true;
				CollinIndex = i;
			}
			break;

			// spostamento prematuro al di fuori della funzione
		case L';':
			if (location == FUNCTION__ and saved == tabs)
			{
				location = OUT__;
			}
			break;
		}
	}

	// output lista delle macro
	wofstream output("hierarchy.txt", ios::trunc);
	if (!output.is_open())
	{
		wcerr << L"Errore nell'apertura del secondo file per la scrittura.\n";
		return 0;
	}
	for (const auto& macro : macros)
	{
		output << L"MACRO  " << macro << L'\n';
	}

	// tokenizzazione iniziale
	int ParenthesisBalance{};
	size_t CharIndex{};
	Textlines.clear();
	for (long long i = Text.size() - 1; i > 0; --i)
	{
		// per andare a capo dopo una funzione
		if (Text.at(i - 1) == L'{' and Text.at(i) == L'}')
		{
			if (i != Text.size() - 1)
			{
				if (Text.at(i + 1) == L',')
				{
					continue;
				}
			}
			Text.insert(Text.begin() + i + 1, L';');
		}
	}
	for (size_t i = 0; i < Text.size() - 1; ++i)
	{
		// separazione righe in base ai punti e virgola
		if (Text.at(i) == L';')
		{
			Textlines.push_back(Text.substr(CharIndex + 1, i - CharIndex - 1));
			CharIndex = i;
			continue;
		}
	}

	// miglioramenti della tokenizzazione
	if (Textlines[0].at(0) == L' ')
	{
		Textlines[0].erase(0, 1);
	}
	for (auto& line : Textlines)
	{
		if (line.empty())
		{
			continue;
		}
		if (line.at(0) == L' ')
		{
			line.erase(0, 1);
			if (line.empty())
			{
				continue;
			}
		}
		if (line.at(line.size() - 1) == L' ')
		{
			line.erase(line.size() - 1);
			if (line.empty())
			{
				continue;
			}
		}
		if (line.at(line.size() - 1) == L';')
		{
			line.erase(line.size() - 1);
		}
	}

	// andamento a capo dopo alcune parentesi graffe
	for (size_t i = 0; i < Textlines.size() - 1; ++i)
	{
		// ricavo della prima parola
	WordConstruction:
		auto WordEnd{ Textlines[i].size() - 1 };
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			if (!isalnum(Textlines[i].at(j)))
			{
				WordEnd = j;
				break;
			}
		}
		auto Word{ Textlines[i].substr(0, WordEnd) };

		// filtro keyword
		if (Word == L"public" or Word == L"private" or Word == L"protected")
		{
			Textlines[i].erase(0, Word.size() + 1);
			if (Textlines[i].empty() or Textlines[i].empty())
			{
				Textlines.erase(Textlines.begin() + i--);
				continue;
			}
			if (Textlines[i].at(0) == L' ')
			{
				Textlines[i].erase(0, 1);
			}
			goto WordConstruction;
		}
		if (Word != L"namespace" and Word != L"class" and
			Word != L"struct" and Word != L"union" and Word != L"template")
		{
			continue;
		}

		// annotazione della posizione della parentesi graffa
		auto SeparationIndex{ Textlines[i].find(L'{') };
		if (SeparationIndex == wstring::npos)
		{
			continue;
		}

		// esclusione funzioni template
		bool Continue{ false };
		for (long long j = SeparationIndex - 1; j >= 0; --j)
		{
			if (Textlines[i].at(j) != L' ')
			{
				Continue = Textlines[i].at(j) == L')';
				break;
			}
		}
		if (Continue)
		{
			continue;
		}

		// spostamento a capo ed eliminazione di eventuali spazi
		wstring NewLine{ Textlines[i].substr(SeparationIndex + 1) };
		if (NewLine.empty())
		{
			Textlines[i++].erase(SeparationIndex + 1);
			continue;
		}
		if (NewLine.at(0) == L' ')
		{
			NewLine.erase(0, 1);
			if (NewLine.empty())
			{
				Textlines[i++].erase(SeparationIndex + 1);
				continue;
			}
		}
		Textlines.insert(Textlines.begin() + i + 1, NewLine);
		Textlines[i].erase(SeparationIndex + 1);
	}

	// eliminazione di alcune righe vuote
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		if (Textlines[i].empty() or Textlines[i] == L" ")
		{
			Textlines.erase(Textlines.begin() + i);
		}
	}

	// spezzamento delle inizializzazioni di più variabili in un'istruzione
	int balance1{}, balance2{}, balance3{};
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			switch (Textlines[i].at(j))
			{
			case L'{':
				if (i == 553)
				{
					i = i;
				}
				if (j != Textlines[i].size() - 1)
				{
					balance1++;
				}
				break;
			case L'}':
				if (balance1 > 0)
				{
					balance1--;
				}
				break;
			case L'(':
				balance2++;
				break;
			case L')':
				balance2--;
				break;

			case L'<': {
				// skip dei nomi degli operatori
				long long OperatorIndex{ -1 };
				for (long long k = j - 1; k >= 0; --k)
				{
					if (Textlines[i].at(k) != L'<' and
						Textlines[i].at(k) != L'=' and
						Textlines[i].at(k) != L'>')
					{
						OperatorIndex = k;
						break;
					}
				}
				if (OperatorIndex > 8)
				{
					if (Textlines[i].substr(OperatorIndex - 7, 8) == L"operator")
					{
						break;
					}
				}
			}
				balance3++;
				break;

			case L'>': {
				// skip dei nomi degli operatori
				long long OperatorIndex{ -1 };
				for (long long k = j - 1; k >= 0; --k)
				{
					if (Textlines[i].at(k) != L'<' and
						Textlines[i].at(k) != L'=' and
						Textlines[i].at(k) != L'>')
					{
						OperatorIndex = k;
						break;
					}
				}
				if (OperatorIndex > 8)
				{
					if (Textlines[i].substr(OperatorIndex - 7, 8) == L"operator")
					{
						break;
					}
				}
			}
				balance3--;
				break;

			case L',':
				if (balance1 != 0 or balance2 != 0 or balance3 != 0)
				{
					break;
				}

				// calcolo del datatype
				int AnotherBalance{};
				wstring datatype{ Textlines[i].substr(0, j) };
				for (long long k = datatype.size() - 1; k >= 0; --k)
				{
					switch (datatype.at(k))
					{
					case L'{':
						AnotherBalance++;
						break;
					case L'}':
						AnotherBalance--;
						break;
					case L' ':
						if (AnotherBalance == 0)
						{
							datatype.erase(k);
						}
						break;
					}
				}

				// spostamento a capo in base alle virgole
				for (long long k = Textlines[i].size() - 1; k >= 0; --k)
				{
					if (Textlines[i].at(k) == L',')
					{
						auto NewWord{ Textlines[i].substr(k + 1) };
						if (NewWord.at(0) != L' ')
						{
							NewWord = L' ' + NewWord;
						}
						Textlines.insert(
							Textlines.begin() + i + 1, datatype + NewWord
						);
						Textlines[i].erase(k);
					}
				}

				j = 0;
				break;
			}
		}
	}

	// apertura parentesi graffe
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		for (size_t j = 0; j < Textlines[i].size() - 1; ++j)
		{
			if (Textlines[i].at(j) == L'{' and Textlines[i].at(j + 1) != L'}')
			{
				wstring NewLine{ Textlines[i].substr(j + 1) };
				if (NewLine.at(0) == L' ')
				{
					NewLine.erase(0, 1);
				}
				Textlines.insert(Textlines.begin() + i + 1, NewLine);
				Textlines[i++].erase(j + 1);
			}
		}
	}

	// chiusura di altre parentesi graffe
	for (long long i = Textlines.size() - 1; i > 0; --i)
	{
		if (Textlines[i] == L"}" and
			Textlines[i - 1].at(Textlines[i - 1].size() - 1) == L'{')
		{
			Textlines[i - 1] += L"}";
			Textlines.erase(Textlines.begin() + i);
		}
	}

	// spostamento a capo di una parentesi graffa chiusa
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		// per capire se l'azione è necessaria
		bool push{ false };
		if (Textlines[i].size() == 1)
		{
			push = Textlines[i] == L"}";
		}
		else if (Textlines[i].at(Textlines[i].size() - 1) == L'}' and
			Textlines[i].at(Textlines[i].size() - 2) != L'{')
		{
			push = true;
		}
		
		// esecuzione
		if (push)
		{
			Textlines[i].erase(Textlines[i].size() - 1);
			Textlines.insert(Textlines.begin() + i + 1, L"}");
			
			if (Textlines[i].empty())
			{
				continue;
			}
			if (Textlines[i].at(Textlines[i].size() - 1) == L' ')
			{
				Textlines[i].erase(Textlines[i].size() - 1);
			}
		}
	}

	// rimozione dell'inizializzazione uniforme
	int Balance;
	size_t LineIndex;
	bool balancing{ false };
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		if (Textlines[i].empty())
		{
			Textlines.erase(Textlines.begin() + i--);
		}

		// scorrimento righe
		if (balancing)
		{
			// bilancio necessario per eliminare
			for (size_t j = 0; j < Textlines[i].size(); ++j)
			{
				switch (Textlines[i].at(j))
				{
				case L'{':
					Balance++;
					break;
				case L'}':
					Balance--;
					break;
				}
			}

			// eliminazione del contenuto delle parentesi graffe
			if (Balance == 0)
			{
				balancing = false;
				for (size_t j = LineIndex; j < i; ++j)
				{
					Textlines.erase(Textlines.begin() + LineIndex + 1);
				}
				i = LineIndex;
				Textlines[LineIndex].erase(Textlines[LineIndex].size() - 1);
			}
			continue;
		}

		// calcolo prima parola in un'istruzione
		auto WordEnd{ Textlines[i].size() - 1 };
		for (size_t j = 0; j <= WordEnd; ++j)
		{
			if (!isalnum(Textlines[i].at(j)) and Textlines[i].at(j) != L'_')
			{
				WordEnd = j;
				break;
			}
		}
		auto Word{ Textlines[i].substr(0, WordEnd) };

		// keyword importanti in c++
		if (Word == L"template" or Word == L"class" or Word == L"struct" or
			Word == L"enum" or Word == L"union" or Word == L"namespace")
		{
			continue;
		}

		// rimozione degli alias
		if (Word == L"using")
		{
			Textlines.erase(Textlines.begin() + i--);
			continue;
		}

		// filtro contenuti della riga
		if (Textlines[i].find(L'(') != wstring::npos or
			Textlines[i].find(L'=') != wstring::npos or
			Textlines[i].find(L'{') == wstring::npos)
		{
			continue;
		}

		// inizializzazione uniforme su una singola riga
		if (Textlines[i].find(L'}') != wstring::npos)
		{
			Textlines[i].erase(Textlines[i].size() - 2);
			continue;
		}

		// istruzioni di eliminare
		balancing = true;
		LineIndex = i;
		Balance = 1;
	}

	// rimozione dell'inizializzazione con assegnazione
	for (size_t i = 0; i < Textlines.size() - 1; ++i)
	{
		auto index{ Textlines[i].find(L'=') };
		if (index != wstring::npos and
			Textlines[i].find(L'<') == wstring::npos and
			Textlines[i].find(L"operator") == wstring::npos)
		{
			Textlines[i].erase(index);
			if (Textlines[i].at(Textlines[i].size() - 1) == L' ')
			{
				Textlines[i].erase(Textlines[i].size() - 1);
			}
		}
	}

	// eliminazione dei nomi dei parametri
	for (auto& Line : Textlines)
	{
		// estrazione lista dei parametri
		auto first{ Line.find(L'(') };
		if (first == wstring::npos)
		{
			continue;
		}
		auto last{ Line.find(L')') };
		if (last - first == 1)
		{
			continue;
		}
		auto ParameterList{ Line.substr(first + 1, last - first - 1) };

		// eliminazione degli spazi agli estremi
		size_t firstpos{}, lastpos{};
		if (ParameterList.at(ParameterList.size() - 1) == L' ')
		{
			ParameterList.erase(ParameterList.size() - 1, 1);
		}
		if (ParameterList.at(0) == L' ')
		{
			ParameterList.erase(0, 1);
		}
		auto WordBound{ ParameterList.size() };

		// eliminazione dei nomi
		bool found{ false };
		for (long long i = WordBound - 1; i < ParameterList.size() and i >= 0; --i)
		{
			if (ParameterList.at(i) != L' ' and ParameterList.at(i) != L',')
			{
				continue;
			}
		
			bool comma{ ParameterList.at(i) == L',' };
			if (!found)
			{
				ParameterList.erase(i + comma, WordBound - i - comma);
			}
			found = true;
				
			if (comma)
			{
				WordBound = i;
				found = false;
			}
		}

		// resa degli spazi
		for (size_t i = 0; i < ParameterList.size(); ++i)
		{
			if (ParameterList.at(i) != L',')
			{
				continue;
			}
			if (i < ParameterList.size())
			{
				if (ParameterList.at(i + 1) != L' ')
				{
					ParameterList.insert(ParameterList.begin() + i + 1, L' ');
				}
			}
			if (i > 0)
			{
				if (ParameterList.at(i - 1) == L' ')
				{
					ParameterList.erase(i - 1, 1);
					i--;
				}
			}
		}

		Line.replace(first + 1, last - first - 1, ParameterList);
	}

	// rimozione funzioni vuote
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		if (Textlines[i].find(L'(') != wstring::npos and
			Textlines[i].find(L'{') == wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
		}
	}

	// espansione degli elementi in un'enumerazione
	bool Enumerating{ false };
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		// fine del ritaglio
		if (Textlines[i] == L"}" and Enumerating)
		{
			Enumerating = false;
		}
		if (Textlines[i].size() < 4)
		{
			continue;
		}

		// inizio del ritaglio
		if (Textlines[i].substr(0, 4) == L"enum")
		{
			Enumerating = true;
			continue;
		}

		// ritaglio ed espansione
		if (Enumerating)
		{
			auto pos{ Textlines[i].find(L',') };
			if (pos == wstring::npos)
			{
				Enumerating = false;
				continue;
			}

			// costruzione nuova riga
			auto NewLineToWrite{ Textlines[i].substr(pos + 1) };
			if (NewLineToWrite.empty() or NewLineToWrite == L" ")
			{
				Textlines[i].erase(Textlines[i].size() - 1);		
				continue;
			}

			// scrittura di una nuova riga
			if (NewLineToWrite.at(0) == L' ')
			{
				NewLineToWrite.erase(0, 1);
			}
			Textlines.insert(Textlines.begin() + i + 1, NewLineToWrite);
			Textlines[i].erase(pos);
			if (Textlines[i].at(Textlines[i].size() - 1) == L' ')
			{
				Textlines[i].erase(Textlines[i].size() - 1);
			}
		}
	}

	// rimozione dei keyword di template
	for (size_t i = 0; i < Textlines.size(); ++i)
	{
		if (Textlines[i].size() < 8)
		{
			continue;
		}
		if (Textlines[i].substr(0, 8) != L"template")
		{
			continue;
		}

		// ricerca del keyword typename o class
		auto TypenameIndex{ Textlines[i].find(L"typename") };
		if (TypenameIndex == wstring::npos)
		{
			TypenameIndex = Textlines[i].find(L"class") + 5;
		}
		else
		{
			TypenameIndex += 8;
		}

		// calcolo del nome del typename
		while (Textlines[i].at(TypenameIndex) == L' ')
		{
			TypenameIndex++;
		}
		size_t EndIndex{ TypenameIndex + 1 };
		for (;;)
		{
			if (Textlines[i].at(EndIndex) == L' ' or
				Textlines[i].at(EndIndex) == L'>')
			{
				break;
			}
			++EndIndex;
		}
		auto TpName{ Textlines[i].substr(TypenameIndex, EndIndex - TypenameIndex) };
		
		// rimozione della dichiarazione di template
		Textlines[i].erase(0, Textlines[i].find(L'>') + 1);
		if (Textlines[i].at(0) == L' ')
		{
			Textlines[i].erase(0, 1);
		}

		// se la classe è derivata
		long long DerivationIndex{ -1 };
		for (size_t j = 1; j < Textlines[i].size() - 1; ++j)
		{
			if (Textlines[i].at(j) == L':' and
				Textlines[i].at(j + 1) != L':' and
				Textlines[i].at(j - 1) != L':')
			{
				DerivationIndex = j;
				break;
			}
		}
		if (DerivationIndex > 0)
		{
			Textlines[i].erase(
				DerivationIndex,
				Textlines[i].size() - DerivationIndex - 1
			);
		}

		// se il template è specificato
		size_t TpNameIndexing{ Textlines[i].find(L'<' + TpName + L'>') };
		if (TpNameIndexing != wstring::npos)
		{
			TpNameIndexing += TpName.size() + 2;
			while (Textlines[i].at(TpNameIndexing) == L' ')
			{
				TpNameIndexing++;
			}
			if (Textlines[i].at(TpNameIndexing) == L'(')
			{
				continue;
			}
		}

		// ricerca del nome della funzione
		auto Char{ Textlines[i].find(L'(') };
		if (Char == wstring::npos)
		{
			Char = Textlines[i].size() - 1;
		}
		auto EndOfLine{ Textlines[i].substr(Char) };
		Textlines[i].erase(Char);
		
		// operazioni finali
		auto IndexOfSupposedSpace{ Textlines[i].size() - 1 };
		Textlines[i] += L'<' + TpName + L'>' + EndOfLine;
		if (Textlines[i].at(IndexOfSupposedSpace) == L' ')
		{
			Textlines[i].erase(IndexOfSupposedSpace, 1);
		}
	}

	// rimozione delle cose vuote
	for (long long i = Textlines.size() - 1; i >= 0; --i)
	{
		// calcolo della prima parola
		long long FirstSpace{ -1 };
		for (size_t j = 0; j < Textlines[i].size(); ++j)
		{
			if (!isalpha(Textlines[i].at(j)))
			{
				FirstSpace = j;
			}
		}
		if (FirstSpace < 0)
		{
			continue;
		}
		auto Word{ Textlines[i].substr(0, FirstSpace) };

		// controllo
		if ((Word == L"class" or Word == L"struct" or Word == L"enum"
			or Word == L"union" or Word == L"namespace") and
			Textlines[i].find(L'{') == wstring::npos)
		{
			Textlines.erase(Textlines.begin() + i);
		}
	}

	// traduzione riga per riga e output finale
	wstring LastString;
	output << L'\n';
	int IndentationTabs{};
	for (auto Line : Textlines)
	{
		// parentesi
		if (Line == L"}")
		{
			output << L'\n';
			IndentationTabs--;
			LastString = L"}";
			continue;
		}

		// variabile di un enum
		long long space{ -1 };
		for (size_t i = 0; i < Line.size(); ++i)
		{
			if (!isalpha(Line.at(i)))
			{
				space = i;
				break;
			}
		}
		if (space < 0)
		{
			output << wstring(IndentationTabs, L'\t');
			output << L"ENUM_VAR  " << Line << L'\n';
			LastString = Line;
			continue;
		}
		auto Word{ Line.substr(0, space) };

		// enum
		if (Word == L"enum")
		{
			if (LastString != L"}")
			{
				output << L'\n';
			}
			output << wstring(IndentationTabs, L'\t') << L"ENUM ";
			Line.erase(0, Word.size());
			if (Line.at(0) == L' ')
			{
				Line.erase(0, 1);
			}

			// enum class
			if (Line.size() >= 5)
			{
				if (Line.substr(0, 5) == L"class")
				{
					output << L"CLASS  ";
					Line.erase(0, 5);
					if (Line.at(0) == L' ')
					{
						Line.erase(0, 1);
					}
				}
				else
				{
					output << L' ';
				}
			}
			else
			{
				output << L' ';
			}

			auto FirstSpace{ Line.find(L' ') };
			if (FirstSpace != wstring::npos)
			{
				Line.erase(FirstSpace);
			}

			output << Line << L":\n";
			IndentationTabs++;
			LastString = Line;
			continue;
		}

		// varie cose
		if (Word == L"class" or Word == L"struct" or
			Word == L"enum" or Word == L"namespace")
		{
			// output tipo
			auto Uppercase{ Word };
			for (auto& ch : Uppercase)
			{
				ch = toupper(ch);
			}

			if (LastString != L"}")
			{
				output << L'\n';
			}
			output << wstring(IndentationTabs, L'\t');
			output << Uppercase << L"  ";

			// eliminazione primo spazio
			Line.erase(0, Word.size());
			if (Line.at(0) == L' ')
			{
				Line.erase(0, 1);
			}

			// eliminazione spazzature
			size_t FirstSpace{ wstring::npos };
			for (size_t i = 1; i < Line.size() - 1; ++i)
			{
				if (Line.at(i) == L':' and
					Line.at(i + 1) != L':' and
					Line.at(i - 1) != L':')
				{
					FirstSpace = i;
					break;
				}
			}
			if (FirstSpace == wstring::npos)
			{
				FirstSpace = Line.find(L'{');
			}
			Line.erase(FirstSpace);

			// eliminazione ultimo spazio
			if (Line.at(Line.size() - 1) == L' ')
			{
				Line.erase(Line.size() - 1);
			}

			// output nome
			output << Line << L":\n";
			IndentationTabs++;
			LastString = Line;
			continue;
		}

		// funzione
		if (Line.find(L'(') != wstring::npos)
		{
			Line.erase(Line.find(L')') + 1);
			output << wstring(IndentationTabs, L'\t');
			output << L"FUNCT  " << Line << L'\n';
			LastString = Line;
			continue;
		}

		// variabile globale
		output << wstring(IndentationTabs, L'\t') << L"VAR  " << Line << L'\n';
		LastString = Line;
	}
	output.close();
	return 0;
}