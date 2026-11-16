#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LArmL
		Sound Seers_SootheMind_Attack 100 100 1
		BhvrOverride
			PyrRotate	0 90 0
		End
		Part	:Arm_Rings_Traveling_Left.part
		Lifespan 	45
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PyrRotate	0 90 0
		End
		Part	:Arm_Rings_Traveling_Right.part
		Lifespan 45
		LifespanJitter	10
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Hands_Mist.part
		Lifespan 	50
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Hands_Mist.part
		Lifespan 50
	End
End

##############################################################################

End