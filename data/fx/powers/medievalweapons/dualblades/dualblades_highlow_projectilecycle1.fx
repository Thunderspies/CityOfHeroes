#############################################################
## DualBlades - High Low Attack
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On 	Cycle
	Time 	2.5

	Event
		EName	Projectile1
		Type	Local
		At	Origin
		ChildFX	:DualBlades_HighLow_Projectile1.fx
		LifeSpan	45
	End
End

#############################################################

End