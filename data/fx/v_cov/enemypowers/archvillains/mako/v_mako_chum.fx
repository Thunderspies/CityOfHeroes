#########################################################
##	Fireweapon
########################################################
FxInfo
LifeSpan	140  
######################################################################



Condition
	On Time
	Time 8
	Event
		Type	Local
		At	Origin 
		Sound ChumSpew1 95 95 .9
	End
End


Condition
	On 	Time
	Time 	32

	Event
		
		Type	positonly
		At	head
		ename	header
		lookat	T_head
		lifespan 50
	end

	Event
		
		Type	local
		At	header
#		BHVR	:WaterSprayOffset.bhvr
		part	:V_Mako_ChumWater.part
		part	:V_Mako_ChumWaterRed.part
		part	:V_Mako_ChumChunks.part
		bhvroverride
			PositionOffset		0 0 -2
		end
	End
	Event
		
		Type	local
		At	header
		part	:V_Mako_ChumSpittle.part
		part	:V_Mako_ChumSpittleRed.part
	End


	Event
		Ename	PushForce
		Type	Local
		
		At	header
		
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	100
		End
		

		LifeSpan	75

	End

	
	Event
		Ename	PushForce2
		Type	Local
		
		At	header
		
		BhvrOverride
			PositionOffset		0 0 25
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		75
			PhysForcePowerJitter	50
		End
		

		LifeSpan	75

	End
End

############# Drool

Condition
	On 	Time
	Time 	0


	Event
		
		Type	local
		At	jaw
		ename	jawfront
#		geom	testing_Target
		bhvroverride
			PositionOffset		0 -.2 .3
		end
	end	
	Event
		
		Type	local
		At	jaw
		ename	jawsideL
#		geom	testing_Target
		bhvroverride
			PositionOffset		-.18 -.1 .1
		end
	end
	Event
		
		Type	local
		At	jaw
		ename	jawsideR
#		geom	testing_Target
		bhvroverride
			PositionOffset		.18 -.1 .1
		end
	end

	Event
		Type	local
		At	jawfront
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		pother	jawsideL
		lifespan 75
	end
	Event
		Type	local
		At	jawfront
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		pother	jawsideR
		lifespan 75
	end


end

Condition
	On 	Time
	Time 	20
	Event
		Type	local
		At	jawfront
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		pother	jawsideL
		lifespan 100
	end
	Event
		Type	local
		At	jawfront
		part	:V_Mako_ChumDrool.part
		part	:V_Mako_ChumDroolRed.part
		pother	jawsideR
		lifespan 100
	end

end



End
##################################

