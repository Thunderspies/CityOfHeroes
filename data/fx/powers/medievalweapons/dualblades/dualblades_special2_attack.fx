#############################################################
## DualBlades - Special 2 Attack
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Mystic
		Sound Whoosh5 100.0 100.0 0.25
	End

	Event
		Type	Start
		At	Mystic
		Geom	CrossSwoosh1
		bhvr	:DualBlade_CrossSwoosh1.bhvr
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOffset
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset		0.0 4.0 0.0
		End
		##Geom	GEO_FX_TestSphere
	End

	Event
		EName	ProjectileStartOffset
		Type	Start
		At	Chest
		LookAt	TargetOffset
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	Projectile1
		Type	Local
		At	ProjectileStartOffset
		part	:DualBlades_Projectile2.part
		part	:DualBlades_Projectile3.part
	End
End

#############################################################

End