#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	PositOnly
		At	Chest
		Part	:Sparks_Fire.part
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:Fire_Fire_Forward.part
		Part	:Fire_Mult.part
		Part	:Fire_Fire_Forward.part
	End

	Event
		Type	PositOnly
		At	LLegL
		Part	:Fire_Fire_Forward_Small.part
		Part	:Fire_Mult.part

	End

	Event
		Type	PositOnly
		At	LLegR
		Part	:Fire_Fire_Forward_Small.part
		Part	:Fire_Mult.part

	End

End

#########################################################

End