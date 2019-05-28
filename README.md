# CAL-Project

Instruções para utilizar o projeto:
- Colocar os ficheiros relativos aos mapas numa pasta GraphFiles, que se encontre no mesmo diretório que a pasta src
- Os ficheiros devem estar organizados da seguinte forma:
    [Pasta com nome do mapa] (ex: Porto)
      [T08_edges_<nome do mapa>.txt] (ex: T08_edges_Porto.txt)
      [T08_nodes_lat_lon_<nome do mapa>.txt] (ex: T08_nodes_lat_lon_Porto.txt)
- Colocar o GraphViewerController.jar no pwd (No caso de Clion, IDE que usamos para desenvolver o projeto, colocar na pasta cmake-build-debug do projeto)

O projeto deve abrir automaticamente uma janela do GraphViewer, e se escolher a opção loadMaps esta deve listar todos os mapas posseivei
(Ou seja, todas as pastas dentro da pasta GraphFiles)

Sempre que terminar o programa fechar também a janela do GraphViewer
