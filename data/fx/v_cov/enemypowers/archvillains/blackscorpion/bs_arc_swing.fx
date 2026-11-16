#########################################################
#	
#

FxInfo
LifeSpan  60

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

#################################


Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	origin
		Sound scorpionwhoosh 70 70 .88
	End

end


Condition
	On 	Time
	Time 	26
	Event
		ename	CANNON
		Type	Local 
		At	C_Hind_LLegR
		geom    Testing_Target	
		part	:BS_Arc_Streak.part
		part	:BS_Arc_Streak_Fire.part
		part	:BS_Arc_Streak_Fire.part
		part	:BS_Arc_Streak_Fire.part
		BhvrOverride
			PositionOffset		0 0 -.8
		End
		lifespan 09
		pother C_Hind_LLegR
	End


	Event
		Type	Local 
		At	C_Hind_LLegR
		part	:BS_Arc_Streak.part
		lifespan 09
		pother C_Hind_ULegR
	End

	Event
		Type	Local 
		At	C_Hind_ULegR
		part	:BS_Arc_Streak.part
		lifespan 09
		pother C_Hind_ToeL
	End
	Event
		Type	Local 
		At	C_Hind_ToeL
		part	:BS_Arc_Streak.part
		lifespan 09
		pother C_Hind_FootL
	End
	Event
		Type	Local 
		At	C_Hind_FootL
		part	:BS_Arc_Streak.part
		lifespan 09
		pother C_Hind_LlegL
	End
	Event
		Type	Local 
		At	C_Hind_LlegL
		part	:BS_Arc_Streak.part
		lifespan 09
	End


end

Condition
	On 	Time
	Time 	26
	Event
		Type	Local 
		At	LarmL
		part	:BS_Arc_Streak_Arm.part
		lifespan 09
		pother UarmL
	End
	Event
		Type	Local 
		At	larml
		part	:BS_Arc_Streak_Arm.part
		part	:BS_Arc_Streak_Fire.part
		part	:BS_Arc_Streak_Fire.part
		part	:BS_Arc_Streak_Fire.part
		lifespan 09
		pother LarmL
		BhvrOverride
			PositionOffset		-1.5 0 0
		End
	End

end

Condition
	On 	Time
	Time 	30
	Event
		Type	local  ##circular streak for hand
		At	root
		part	:BS_Arc_Streak_Wave.part
#		lifespan 09
		BhvrOverride
			PositionOffset		0 4.85 0
			PYRrotate	0 -65 0
			spin		0 -.1 0
		End
	End
	Event
		Type	local  ##circular streak for tail
		At	root
		part	:BS_Arc_Streak_Wave_Tail.part
		BhvrOverride
			PositionOffset		0 3.1 0
			PYRrotate	0 -35 0
			spin		0 -.9 0
		End
	End

end

End				