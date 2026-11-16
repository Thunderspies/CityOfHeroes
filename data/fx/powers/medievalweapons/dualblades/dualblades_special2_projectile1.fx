#############################################################
## DualBlades - High Low Attack
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Projectile1
		Type	Local
		At	Chest
		part	:DualBlades_Projectile1.part
		pMagnet	Target
	End
End

#############################################################

End