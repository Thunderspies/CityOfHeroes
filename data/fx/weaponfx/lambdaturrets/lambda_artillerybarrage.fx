#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 36

############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	10 0 10
		End
		ChildFX	:Child_Barrage_Attack.fx
		Lifespan	30
	End
End

Condition
	On	Cycle
	Time	12

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	10 0 10
		End
		ChildFX	:Child_Barrage_Attack.fx
		Lifespan	30
	End
End

#############################################################

End