#############################################################
## FX System Name
#############################################################

FxInfo

## JUMP ###########################################################

Condition
	On		TriggerBits
	TriggerBits	Jump
	DoMany		1

	Event
		EName	LeftBoot
		Type	local 
		At	LlegL
		While	Air
		Flags	OneAtATime
		Part	:Smoke.part
	End

	Event
		EName	RightBoot
		Type	local
		At	LlegR
		While	Air 
		Flags	OneAtATime
		Part	:Smoke.part
	End
End

## LAND ###########################################################

Condition
	On		TriggerBits
	TriggerBits	concrete
	TriggerBits	grass
	TriggerBits	gravel
	TriggerBits	Dirt
	TriggerBits	Wood
	TriggerBits	Carpet
	TriggerBits	metal
	DoMany		1
		
	Event
		EName	Landing
		Type	Local 
		At	Root
		Until	Air
		Flags	OneAtaTime
		Part	:SmokeLanding.part
		#Part	:SmokeLandingLight.part
		Part	:SmokeLanding_Decal.part
	End
End

#############################################################

End				