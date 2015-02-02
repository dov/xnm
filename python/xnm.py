#!/usr/bin/env python
#
# a parser for the xnm file format.
#
# Copyright (c) 2014 Dov Grobgeld <https://github.com/dov>
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation; either version 2.1 of the License, or (at your
# option) any later version.
# 
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
# An xnm module in python with an interface similar to json
#
from pyparsing import *
import re

# The following "magic" allows "dot access" to xnm files.
class dotdict(dict):
    def __getattr__(self, attr):
        return self.get(attr)

    def __dir__(self):
        return self.keys()

    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

def convertNumbers(s,l,toks):
    n = toks[0]
    try:
        return int(n)
    except ValueError, ve:
        return float(n)
        
def parsing_to_pod(results):
    if isinstance(results,ParseResults):
      if results.haskeys():
        return dotdict({
          k : parsing_to_pod(results[k])
          for k in results.keys()
        })
      else:
        return [
          parsing_to_pod(v) for v in results
        ]
    return results

def load(hdl):
    return loads(hdl.read())

def loads(s):
    xnmString = (dblQuotedString.setParseAction( removeQuotes )
                 | sglQuotedString.setParseAction( removeQuotes )
                 | Word(alphas + '_-', alphanums+'_-') )
    xnmNumber = Combine( Optional('-')
                         + ( '0' | Word('123456789',nums) ) +
                        Optional( '.' + Word(nums) ) +
                        Optional( Word('eE',exact=1)
                                  + Word(nums+'+-',nums) ) )
    xnmTable = Forward()
    xnmValue = Forward()
    xnmMembers = Forward()
    xnmArray = Group(Suppress('[') + ZeroOrMore(xnmValue) + Suppress(']') )
    xnmValue << ( xnmNumber | xnmString | xnmTable  | xnmArray )
    memberDef = Group(xnmString + Suppress(oneOf('= : =>')) + xnmValue )
    xnmTable << Dict( Suppress('{') + ZeroOrMore(memberDef) + Suppress('}') )
    
    xnmComment = pythonStyleComment
    xnmTable.ignore( xnmComment )
    xnmNumber.setParseAction( convertNumbers )
    
    return parsing_to_pod(xnmTable.parseString('{'+s+'}'))


def string_protect(s):
    if re.search(r'\W',s):
        return '"'+s+'"'
    return s

def internal_dumps(s, indent=2):
    ret = []
    if isinstance(s,dict):
        ret += ['{']
        for k,v in s.iteritems():
            ret += [ k + ' = ' + internal_dumps(v)]
        ret += ['}']
    elif type(s) == list:
        ret += ['[']
        for v in s:
            ret += [ internal_dumps(v)]
        ret += [']']
    elif type(s) == float:
        ret += ['%f'%s]
    elif type(s) == int:
        ret += ['%d'%s]
    elif isinstance(s,str) or isinstance(s,unicode):
        ret += [string_protect(s)]
    else:
        raise Exception('Unsupported xnm type ' + str(type(s)))
    return ' '.join(ret)

def dumps(s, indent=2):
    if not isinstance(s,dict):
        raise Exception('Top level of xnm must be a dict!')

    ret = []
    for k,v in s.iteritems():
        ret += [ k + ' = ' + internal_dumps(v)]
    return '\n'.join(ret)

if __name__=='__main__':
#    # Testing
#    s =  dumps({'a':3, 'b':[3,4,'bar baz']})
#    t = loads(s)
#    print t
#    print t.a,t.b

    # s = open('test.xnm').read()
    # print loads(s)

    print dumps({'foo': "the qui"})
    
