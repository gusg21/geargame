local map = {}

-- Special object, this is called by the Scene loader if we load a tiled map
function map:set_tiled_map(map)
    -- print("\tGot map " .. tostring(map))
    self.tmap = map
end

function map:draw()
    if not IsNull(self.tmap) and not IsNull(self.window) then
        TiledMap_Draw(self.tmap, self.window, 0, 0)
    end
end

return map