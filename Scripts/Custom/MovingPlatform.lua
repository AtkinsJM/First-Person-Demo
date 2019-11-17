Script.numWaypoints = 1 --choice "Number of waypoints" "0, 1, 2, 3"
Script.waypoint1 = nil --entity "Waypoint 1"
Script.waypoint2 = nil --entity "Waypoint 2"
Script.waypoint3 = nil --entity "Waypoint 3"
Script.speed = 1 --float "Speed"
Script.automated = true --bool "Automatic"
Script.endBehaviour = Loop --choiceedit "End Behaviour" "Loop, Reverse, Restart"


function Script:Start()
	self.entity:SetKeyValue("tag", "MovingPlatform")
	self.entity:SetKeyValue("numWaypoints", self.numWaypoints)
	local string waypointName = ""
	if self.waypoint1~=nil then
		waypointName =self.waypoint1:GetKeyValue("name")
		self.entity:SetKeyValue("waypoint1", waypointName)
	end
	if self.waypoint2~=nil then
		waypointName =self.waypoint2:GetKeyValue("name")
		self.entity:SetKeyValue("waypoint2", waypointName)
	end
	if self.waypoint3~=nil then
		waypointName =self.waypoint3:GetKeyValue("name")
		self.entity:SetKeyValue("waypoint3", waypointName)
	end
	self.entity:SetKeyValue("speed", self.speed)
	self.entity:SetKeyValue("automatic", self.automatic)
	self.entity:SetKeyValue("endBehaviour", self.endBehaviour)
end


--[[
function Script:UpdateWorld()
	
end
]]

--[[
function Script:UpdatePhysics()
	
end
]]

--[[
--This can be used to select which objects an entity collides with.  This overrides collision types completely.
function Script:Overlap(e)
	return Collision:Collide
end
]]

--[[
function Script:Collision(entity, position, normal, speed)
	
end
]]

--[[
function Script:Draw()
	
end
]]

--[[
function Script:DrawEach(camera)
	
end
]]

--[[
--This function will be called after the world is rendered, before the screen is refreshed.
--Use this to perform any 2D drawing you want the entity to display.
function Script:PostRender(context)
	
end
]]

--[[
--This function will be called when the entity is deleted.
function Script:Detach()
	
end
]]

--[[
--This function will be called when the last instance of this script is deleted.
function Script:Cleanup()
	
end
]]

--[[
--This function will be called upon completion of a one-shot animation played with the PlayAnimation() command.
function Script:EndAnimation(sequence)
	
end
]]