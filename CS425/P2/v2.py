#!/usr/bin/python3

import re
import sys
import requests
import subprocess
from pprint import pprint
from bs4 import BeautifulSoup

def main(category, word1, word2):
  # UNCOMMENT FOR USE WITH WORDNET SOFTWARE
  # p = subprocess.check_output(category)

  # use adjacency list to represent graph (using sets to prevent duplicates)
  graph = {}
  graph[category] = set()

  # TESTING: sample input file
  with open('test.txt','r') as fp:
      raw_data = fp.read()

  # extract sense 1
  regex = re.compile(r'Sense\s3\n(.*?)Sense', flags=re.DOTALL)
  sense_data = regex.search(raw_data)
  if not sense_data:
      raise Exception('Word not found.')
  
  # regex out items
  match = sense_data.group(1)
  lines = [_ for _ in match.splitlines() if _.startswith(' ')]
  
  # start with category and 7 spaces as first level
  prev = last = category
  n_spaces = 7

  for line in lines:
    spaces, _, words = line.partition('=> ')
    nxt = words.split(',')[0] # hard coding first word for now

    # add both relationships (prev -> nxt, nxt -> prev)
    graph[prev].add(nxt)

    if nxt not in graph:
        graph[nxt] = {prev}
    else:
        graph[nxt].add(prev)

    # add both relationships (prev -> nxt, nxt -> prev)
    if len(spaces) > n_spaces:
        prev = last
    elif len(spaces) < n_spaces:
        prev = category
    n_spaces = len(spaces)
    last = nxt

  # display graph
  pprint(graph)
  dist, path = bfs(graph, word1, word2)
  if dist == float('inf'):
    path = 'no path found'

  # display semantic distance and path from word1 to word2
  print(f'\nSemantic distance between "{word1}" and "{word2}": {dist}')
  print('Path', path, '\n')
  
def bfs(graph, source, target):
  minDist = float('inf')
  q = [(source, 0, [source])]
  visited = set()
  while q:
    curr_word, curr_dist, curr_path = q.pop(0)
    if curr_word == target:
      minDist = min(minDist, curr_dist)
      break
    visited.add(curr_word)
    if curr_word in graph:
      for next_word in graph[curr_word]:
        if next_word not in visited:
          q.append((next_word, curr_dist + 1, curr_path + [next_word]))
  return minDist, curr_path


if __name__ == '__main__':
  main('run', 'marathon', 'mile')
