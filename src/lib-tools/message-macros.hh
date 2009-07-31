/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MESSAGE_MACROS_HH
#define MESSAGE_MACROS_HH

//******************************************************************************
/* Bibliothèque permettant de debugger un programme à l'aide de messages
 *
 * Fonctionnement :
 * ----------------
 *  - des messages simples peuvent être affichés à l'aide de la macro MSG()
 *  - il est possible de connaître les fonctions ou bouts de code empruntés par
 *    le programme au cours de son execution en utilisant les macros ENTER et
 *    EXIT (ou bien BEGIN() et END()) au début et à la fin de chaque fonction
 *    (ATTENTION : pour que cela fonctionne correctement, les macros EXIT et
 *    END doivent être placées avant le return de la fonction !).
 *  - les messages sont désactivés par défaut et peuvent être activés en plaçant
 *    un '#define DEBUG_MESSAGES' juste avant l'inclusion de ce fichier.
 *
 * Exemple :
 * ---------
 * #define DEBUG_MESSAGES
 * #include "message-macros.hh"
 *
 * int abs(int i)
 * {
 *   ENTER; // ou BEGIN("abs"); ou DEBUG_FUNCTION;
 *
 *   int result;
 *
 *   if (i >= 0) {
 *     MSG("cas où i est positif ou nul");
 *     result = i;
 *   }
 *   else {
 *     MSG("cas où i est négatif");
 *     result = -i;
 *   }
 *
 *   EXIT; // ou END("abs");
 *
 *   return result;
 * }
 */
//******************************************************************************

#include <iostream>
#include <cassert>
#include <string>

void DBG_MSG_SPACES(int n);

//******************************************************************************

// #define DEBUG_MESSAGES

#ifdef DEBUG_MESSAGES

#define MSG(s)   (DBG_MSG_SPACES(0),  std::cout << s << std::endl)
#define TRACE(s) (DBG_MSG_SPACES(0),  std::cout << #s, std::cout.flush(), s, \
		  std::cout << " ==> OK" << std::endl)
#define BEGIN(s) (DBG_MSG_SPACES(1),  std::cout << "==> " << s << std::endl)
#define END(s)   (DBG_MSG_SPACES(-1), std::cout << "<== " << s << std::endl)

#ifdef __ASSERT_FUNCTION
#define ENTER    (DBG_MSG_SPACES(1),  std::cout << "==> " << __ASSERT_FUNCTION\
                                                << std::endl)
#define EXIT     (DBG_MSG_SPACES(-1), std::cout << "<== " << __ASSERT_FUNCTION\
                                                << std::endl)
#else
#define ENTER    (DBG_MSG_SPACES(1),  std::cout << "==> " << __FILE__\
		                                << ":" << __LINE__ << std::endl)
#define EXIT     (DBG_MSG_SPACES(-1), std::cout << "<== " << __FILE__\
		                                << ":" << __LINE__ << std::endl)
#endif

class CDebugFunction
{
 public:
  CDebugFunction(const std::string & AFunctionName, int ALineNumber = 0)
    : FFunctionName(AFunctionName)
  {
    if (ALineNumber > 0)
      BEGIN(FFunctionName << ":" << ALineNumber);
    else
      BEGIN(FFunctionName);
  }

  ~CDebugFunction() { END(FFunctionName); }

 private:
  std::string FFunctionName;
};

#ifdef __ASSERT_FUNCTION
#define DEBUG_FUNCTION CDebugFunction DebugFunctionObject(__ASSERT_FUNCTION)
#else
#define DEBUG_FUNCTION CDebugFunction DebugFunctionObject(__FILE__, __LINE__)
#endif

#else  // DEBUG_MESSAGES

#define MSG(s)
#define TRACE(s) (s)
#define BEGIN(s)
#define END(s)
#define ENTER
#define EXIT

#define DEBUG_FUNCTION

#endif // DEBUG_MESSAGES

//******************************************************************************

#define WARNING_MESSAGES

#ifdef WARNING_MESSAGES

#define WARN_Y(s)    (std::cout << "\033[0;33m" << s << "\033[0m" << std::endl)
#define WARN_B(s)    (std::cout << "\033[0;34m" << s << "\033[0m" << std::endl)
#define WARN_R(s)    (std::cout << "\033[0;35m" << s << "\033[0m" << std::endl)
#define WARN_BY(s)   (std::cout << "\033[1;33m" << s << "\033[0m" << std::endl)
#define WARN_BB(s)   (std::cout << "\033[1;34m" << s << "\033[0m" << std::endl)
#define WARN_BR(s)   (std::cout << "\033[1;35m" << s << "\033[0m" << std::endl)

#ifdef __ASSERT_FUNCTION
#define FUNC_WARN(s) (std::cerr << "Warning: " << __ASSERT_FUNCTION << ": "\
                                << s << std::endl)
#else
#define FUNC_WARN(s) (std::cerr << "Warning: " << __FILE__ << ":" << __LINE__\
                                << ": " << s << std::endl)
#endif

#else // WARNING_MESSAGES

#define WARN_Y(s)
#define WARN_B(s)
#define WARN_R(s)
#define WARN_BY(s)
#define WARN_BB(s)
#define WARN_BR(s)
#define FUNC_WARN(s)

#endif // WARNING_MESSAGES

//******************************************************************************

#endif // MESSAGE_MACROS_HH
