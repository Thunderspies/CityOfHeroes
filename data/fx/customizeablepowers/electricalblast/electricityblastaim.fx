#############################################################
## ElectricityBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	chest
		Childfx	:Child_Aim.fx
		LifeSpan	70
		LifeSpanJitter	40
	End
End

## GENERIC FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\Child_Aim.fx
	End
End

#############################################################

End