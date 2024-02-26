local map = {}

function map:initialize(me, state, window, assets)
    self.state = state
    self.window = window
    self.assets = assets
end

-- Special object, this is called by the Scene loader if we load a tiled map
function map:set_tiled_map(map)
    -- print("\tGot map " .. tostring(map))
    self.tmap = map
end

function map:draw()
    if self.tmap ~= nil then
        TiledMap_Draw(self.tmap, self.window, 0, 0)
    end
end

return map