#########################################################
##	
##

FxInfo
LifeSpan  75

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	0

	Event
		ename	CANNON
		Type	Local 
		At	C_Hind_LLegR
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		0 0 -1.5
		End
	End
	Event
		ename	STARTER
		Type	local 
		At	root
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		.3 4.3 6.1
			PYrRotate		-7 -7 0
			scale	.5 .5 .8
		 End
	End

end
Condition
	On 	Time
	Time 	0

	Event	
		ename	Charge
		Type	local
		At	cannon	
		part	:BS_Sting_chargeball.part
		lifespan 50
	end
	Event	
		ename	smoke
		Type	local
		At	cannon	
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 36
	end
	Event	
		ename	smoke2
		Type	local
		At	cannon	
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 39
	end
	Event	
		ename	disChargeSpot
		Type	local
		At	waist	
		BhvrOverride
			PositionOffset		0 3.2 1.6
		End
	end
	Event	
		ename	disCharge
		Type	positonly
		At	disChargeSpot	
		lookat	T_Chest
	end
	Event	
		ename	jointsmoke
		Type	local
		At	cannon	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 120
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_LLegR	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 85
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_ULegR
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 80
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_ToeL	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 75
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_FootL	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 70
	end


end



Condition
	On 	Time
	Time 	20

	Event
		Type	local 
		At	STARTER	
		childfx :BS_Sting_Tendril.fx
		BhvrOverride
			scale 1 1 1
		End
	End
	Event
		Type	local 
		At	STARTER	
		childfx :BS_Sting_Tendril.fx
		BhvrOverride
			scale 1 1 1
		End
	End
end
	
Condition
	On 	Time
	Time 	23

	Event	
		ename	disCharge
		Type	local
		At	starter	
		part	:BS_Sting_dischargeball.part
		lifespan 50
	end

	Event
		Type	local 
		At	STARTER	
		childfx :BS_Sting_Tendril.fx
		BhvrOverride
			scale 1 1 1
		End
	End
	Event
		Type	local 
		At	STARTER	
		childfx :BS_Sting_Tendril.fx
		BhvrOverride
			scale 1 1 1
		End
	End

	