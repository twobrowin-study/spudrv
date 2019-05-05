#include <iostream>

#include <libspu.hpp>
#include <structure.hpp>

using namespace std;
using namespace SPU;

#define INF    0xf
#define u_cnt  5 

/* Graph representation

       7
 '1' ------ '3' 
  |        /  \ 7
  |      /      \ 
 2|   4 /       '5'
  |   /         / 
  | /   1     / 6
 '2' ------ '4'

*/

/*************************************
  Structures definitions
*************************************/

/* Graph of convergence G */
Structure<> G; // Only field is u

/* Data Fields of G */
Fields<string> G_data({
  { "Adj[u]", 16 }, // Has own Fields
  { "w[u]",   20 }, // Also has own Fields
  { "d[u]",   4  }, // Max distance is 16
  { "p[u]",   4  },
  { "u∈Q",    1  }  // Boolean
});

/* Adj[u] fields */
Fields<> Adj_u({ // Every field is boolean
  { 1, 1 },
  { 2, 1 },
  { 3, 1 },
  { 4, 1 },
  { 5, 1 }
});

/* w[u] fields */
Fields<> w_u({ // Max distance is 16 
  { 1, 4 },
  { 2, 4 },
  { 3, 4 },
  { 4, 4 },
  { 5, 4 }
});

/* Structure of consideration Q */
Structure<string> Q({
  { "d[u]", 4  }, // Same size as in G_data
  { "u",    60 }
});
auto Q_key = Q.keyFields(); // Get Fields to separate Q key

/*************************************
  End of structures definitions
*************************************/

/* Helpers */
void G_init();
void Q_init();
void G_print();
void Q_print();

int main()
{
  cout << "Starting Dijkstra algorithm" << endl;

  /* G */
  G_init();
  G_print();

  /* Q */
  Q_init();
  Q_print();

  /*************************************
    Main algorithm
  *************************************/

  while(Q.get_power())
  {
    /* Get first node from Q and delete it */
    Q_key = Q.min().key;
    u8 u  = Q_key["u"]; // Index of node
    Q.del(Q_key);
    
    /* Get G_data from Q's "u" */
    G.min(); // IDK but this made algorithm work
    G_data = G.search(u).value;
    Adj_u  = G_data["Adj[u]"];
    w_u    = G_data["w[u]"];

    /* Unset u∈Q */
    G_data["u∈Q"] = false;

    /* Check out all v's from Adj[u] */
    for(auto ex : Adj_u.vecData())
    {
      /* If v in Adj[u] */
      if(ex.cont)
      {
        u8 v          = ex.name;
        auto G_data_v = G_data;            // Create new data fields for v node
        G.min();                           // IDK but this made algorithm work
        G_data_v      = G.search(v).value; // Find v's data

        /* v is in Q */
        if(G_data_v["u∈Q"])
        {
          /* Delete old u∈Q */
          Q_key["u"]    = v;
          Q_key["d[u]"] = G_data_v["d[u]"];
          Q.min(); // IDK but this made algorithm work
          Q.del(Q_key);

          /* Create new length statement */
          data_t len = G_data["d[u]"] + w_u[v];
          if( G_data_v["d[u]"] > len )
          {
            /* Set new data */
            G_data_v["d[u]"] = len;
            G_data_v["p[u]"] = u;
          }

          /* Save v state */
          G.insert(v, G_data_v);

          /* Insert new u∈Q */
          Q_key["u"]    = v;
          Q_key["d[u]"] = G_data_v["d[u]"];
          Q.insert(Q_key, 0);
        }
      }
    }

    /* Save u state */
    G.insert(u, G_data);

    G_print();
    Q_print();

    Q.min(); // IDK but this made algorithm work

    cout << "Turn" << endl;
  }

  return 0;
}

/*************************************
  G initialization
  Need to manual set BitFlow class
  in cases of few types in init list
*************************************/
void G_init()
{
  /* u = 1 */
  Adj_u  = { { 2, true }, { 3, true } };
  w_u    = { { 2, 2    }, { 3, 7    } };
  G_data = {
    { "Adj[u]", Adj_u         },
    { "w[u]",   w_u           },
    { "d[u]",   BitFlow(0)    },
    { "p[u]",   BitFlow(0)    },
    { "u∈Q",    BitFlow(true) } 
  };
  G.insert(1, G_data);

  /* u = 2 */
  Adj_u  = { { 1, true }, { 3, true }, { 4, true } };
  w_u    = { { 1, 2    }, { 3, 4    }, { 4, 1    } };
  G_data = {
    { "Adj[u]", Adj_u         },
    { "w[u]",   w_u           },
    { "d[u]",   BitFlow(INF)  },
    { "p[u]",   BitFlow(0)    },
    { "u∈Q",    BitFlow(true) } 
  };
  G.insert(2, G_data);

  /* u = 3 */
  Adj_u  = { { 1, true }, { 2, true }, { 4, true }, { 5, true } };
  w_u    = { { 1, 7    }, { 2, 4    }, { 4, 2    }, { 5, 7    } };
  G_data = {
    { "Adj[u]", Adj_u         },
    { "w[u]",   w_u           },
    { "d[u]",   BitFlow(INF)  },
    { "p[u]",   BitFlow(0)    },
    { "u∈Q",    BitFlow(true) } 
  };
  G.insert(3, G_data);

  /* u = 4 */
  Adj_u  = { { 2, true }, { 3, true }, { 5, true } };
  w_u    = { { 2, 1    }, { 3, 2    }, { 5, 6    } };
  G_data = {
    { "Adj[u]", Adj_u         },
    { "w[u]",   w_u           },
    { "d[u]",   BitFlow(INF)  },
    { "p[u]",   BitFlow(0)    },
    { "u∈Q",    BitFlow(true) } 
  };
  G.insert(4, G_data);

  /* u = 5 */
  Adj_u  = { { 3, true }, { 4, true } };
  w_u    = { { 3, 7    }, { 4, 6    } };
  G_data = {
    { "Adj[u]", Adj_u         },
    { "w[u]",   w_u           },
    { "d[u]",   BitFlow(INF)  },
    { "p[u]",   BitFlow(0)    },
    { "u∈Q",    BitFlow(true) } 
  };
  G.insert(5, G_data);
}

/*************************************
  Q initialization
*************************************/
void Q_init()
{
  /* Q first node init */
  Q.insert(
    { { "d[u]", 0 },  { "u", 1 } },
    0
  );

  /* Q other nodes init */
  for(u8 u=2; u<=u_cnt; u++)
  {
    Q.insert(
      { { "d[u]", INF },  { "u", u } },
      0
    );
  }
}

/*************************************
  G printing
*************************************/
void G_print()
{
  /* Print out */
  cout << "G graph is:" << endl;
  for(u8 u=1; u<=u_cnt; u++)
  {
    pair_t pair =  G.search(u);
    cout << "\t u = " << to_string(pair.key) <<
      ":  " << to_string(pair.value, true) << endl;
  }

  cout << endl;
}

/*************************************
  Q printing
*************************************/
void Q_print()
{
  /* Print out */
  cout << "Q structures keys are:" << endl;

  /* First node */
  pair_t pair = Q.min();
  cout << "\t " << to_string(pair.key, true) << endl;

  /* Other nodes */
  for(u8 u=2; u<=Q.get_power(); u++)
  {
    pair = Q.next(pair.key);
    cout << "\t " << to_string(pair.key, true) << endl;
  }

  cout << endl;
}