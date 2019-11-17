Script.requiredMass = 0 --int "Required Mass"
Script.targetEntity = nil --entity "Target entity"
Script.targetMovement = Vec3(0,2,0) --vec3 "Movement vector"
Script.targetSpeed = 1.0 --float "Movement speed"

function Script:Start()
	self.entity:SetKeyValue("tag", "PressurePlate")
	self.entity:SetKeyValue("requiredMass", self.requiredMass)
	if self.targetEntity~=nil then
       local string targetEntityName = self.targetEntity:GetKeyValue("name")
		self.entity:SetKeyValue("targetEntity", targetEntityName) 
	end
	self.entity:SetKeyValue("targetSpeed", tostring(self.targetSpeed))
	self.entity:SetKeyValue("targetMovementX", tostring(self.targetMovement.x))
	self.entity:SetKeyValue("targetMovementY", tostring(self.targetMovement.y))
	self.entity:SetKeyValue("targetMovementZ", tostring(self.targetMovement.z))
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