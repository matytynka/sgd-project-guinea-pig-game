#include <world.hpp>

World::World(SDL_Renderer * renderer) {
    this->renderer = renderer;
    this->renderX = 0;
    this->renderY = 0;
}

Point World::getRenderP() {
    Point p;
    p.x = renderX;
    p.y = renderY;
    return p;
}

void World::renderWorld() {
    grassBlock = IMG_Load("grass-block.png"); // source: Minecraft
    grassBlockTexture = SDL_CreateTextureFromSurface(renderer, grassBlock);
    flagBlock = IMG_Load("flag.png"); // source: http://pixelartmaker.com/art/7e15e17d2d67d86
    flagBlockTexture = SDL_CreateTextureFromSurface(renderer, flagBlock);
    int sizeY = map.size();
    int sizeX = map.at(0).size();
    for(int mapY = 0; mapY < sizeY; mapY++) {
        for(int mapX = 0; mapX < sizeX; mapX++) {
            SDL_Rect blockRect = {mapX * BLOCKSIZE + renderX, mapY * BLOCKSIZE + renderY, BLOCKSIZE, BLOCKSIZE};
            Block blockId = map.at(mapY).at(mapX);
            switch(blockId) {
                case 0: //pusty blok
                break;
                case 1:
                SDL_RenderCopy(renderer, grassBlockTexture, NULL, &blockRect);
                break;
                case 2:
                SDL_RenderCopy(renderer, flagBlockTexture, NULL, &blockRect);
                break;
            }
        }
    }
    SDL_FreeSurface(grassBlock);
    SDL_FreeSurface(flagBlock);
}

void World::levelLoad(const char *path) {
    inFile.open(path);
    Block block;
    std::vector<Block> row;
    int blockId;
    std::string line;
    while(getline(inFile, line)) {
        std::istringstream ss(line);
        while(ss >> blockId) {
            if(blockId == 0) row.push_back(NOTHING);
            if(blockId == 1) row.push_back(GRASS);
            if(blockId == 2) row.push_back(FLAG);
        }
        map.push_back(row);
        row.clear();
    }
    inFile.close();
}

bool World::checkHitbox(int pigTileX, int pigTileY, int blockX, int blockY) {
    //printf("x1: %d, x2: %d, y1: %d, y2: %d ", pigTileX < (blockX + 1) * BLOCKSIZE, pigTileX + PIG_WIDTH > blockX * BLOCKSIZE, pigTileY < (blockY + 1) * BLOCKSIZE, pigTileY + PIG_HEIGHT > blockY * BLOCKSIZE);
    if (pigTileX <= (blockX + 1) * BLOCKSIZE &&
    pigTileX + PIG_WIDTH >= blockX * BLOCKSIZE &&
    pigTileY <= (blockY + 1) * BLOCKSIZE &&
    pigTileY + PIG_HEIGHT >= blockY * BLOCKSIZE) {
        //printf("pig %i, block %i \n", pigTileY, (blockY + 1) * BLOCKSIZE);
        return true;
    }
    return false;
}

bool World::upperBlocks(int pigX, int pigY) {
    int blockTileX = pigX  / BLOCKSIZE;
    int blockTileY = pigY / BLOCKSIZE;
    for (int i = 0; i < 2; i++) {
        if(pigY < PIG_HEIGHT) { //jak wyleci za wysoko to nic nie rob
            break;
        }

        if(map.at(blockTileY).at(blockTileX + i) == 1) {
            if(checkHitbox(pigX, pigY, blockTileX + i, blockTileY) == true) {
                return true;
                break;
            }
        }

    }
    return false;   
};

bool World::lowerBlocks(int pigX, int pigY, bool * deadPig) {
    int blockTileX = pigX / BLOCKSIZE;
    int blockTileY = (pigY + PIG_HEIGHT) / BLOCKSIZE;
    for (int i = 0; i < 2; i++) {
        //std::cout << pigY  << " " << map.size() * BLOCKSIZE << std::endl;
        if(pigY + PIG_HEIGHT > map.size() * BLOCKSIZE) {
            std::cout << "YOU DIED BY FALLING" << std::endl;
            *deadPig = true;
            break;
        }

        if(map.at(blockTileY).at(blockTileX + i) == 1) {
            if(checkHitbox(pigX, pigY, blockTileX + i, blockTileY) == true) {
                return true;
                break;
            }
        }
    }
    return false;   
};

bool World::rightBlocks(int pigX, int pigY, bool * winPig) {
    int blockTileX = (pigX + PIG_WIDTH) / BLOCKSIZE;
    int blockTileY = (pigY  + (PIG_HEIGHT/2)) / BLOCKSIZE;
    for (int i = 0; i < 1; i++) {
        /*SDL_Rect rect = {blockTileX * BLOCKSIZE, blockTileY * BLOCKSIZE, BLOCKSIZE, BLOCKSIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);*/
        if(map.at(blockTileY + i).at(blockTileX) == 1) {
            if(checkHitbox(pigX, pigY, blockTileX, blockTileY + i) == true){
                //printf("hitbox prawy ");
                return true;
                break;
            }
        }
        if(map.at(blockTileY + i).at(blockTileX) == 2) {
            std::cout << "WIN" << std::endl;
            *winPig = true;
        }

    }
    return false;
};

bool World::leftBlocks(int pigX, int pigY) {
    int blockTileX = pigX  / BLOCKSIZE;
    int blockTileY = (pigY + (PIG_HEIGHT/2)) / BLOCKSIZE;
    for (int i = 0; i < 1; i++) {
        /*SDL_Rect rect = {blockTileX * BLOCKSIZE, blockTileY * BLOCKSIZE, BLOCKSIZE, BLOCKSIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);*/
        if(map.at(blockTileY + i).at(blockTileX) == 1) {
            if(checkHitbox(pigX, pigY, blockTileX, blockTileY + i) == true){
            //printf("hitbox lewy ");
                return true;
                break;
            }
        }

    }
    return false;
};

void World::moveCamera(int pigX, int pigY) {
    /*if(0 - pigY + window_height/2 > 0) {
        renderY = 0;
    } else if(0 - pigY + window_height/2 < 0 - map.size() * BLOCKSIZE + window_height) {
        renderY = 0 - map.size() * BLOCKSIZE + window_height;
    } else {
        renderY = 0 - pigY + window_height/2;
    }*/

    if(0 - pigX + window_width/2 > 0) {
        renderX = 0;
    } else if(0 - pigX + window_width/2 < 0 - map.at(0).size() * BLOCKSIZE + window_width) { //bug
        renderX = 0 - map.at(0).size() * BLOCKSIZE + window_width;
    } else {
        renderX = 0 - pigX + window_width/2;
    }
}