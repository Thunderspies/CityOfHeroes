#############################################################
## Barrage_Hit.fx
#############################################################

FxInfo
LifeSpan 70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	ChestAnchor
		BhvrOverride
			StartJitter	1.0 1.0 0.0
		End
		Part	:Flash_Circle_Small.part
		Part	:Smoke_Circle.part
		Part	:Flash_Small.part
		Part	:Sparks_Hit.part
		Lifespan	2
	End
End

#############################################################

End