#############################################################
## MysticBurst_Attack.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		ChildFX	:Child_BodyGlow_Heal.fx
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound Runesoldier_MysticBurst_Attack_01 100 100 .8
	End
End



Condition
	On 	Time
	Time 	25

	Event
		ENAME   GunOffset
		Type	Start
		At	WepR
		Geom	FlameThrower
	End

	Event
		Type	Start
		At	GunOffset
		AltPiv	1
		Part	:Flash_Core.part
		LifeSpan	5
	End
End

Condition
	On 	Time
	Time 	29

	Event
		Type	start
		At	GunOffset
		AltPiv	1
		Part	:Streaks_Explode_Detached.part
		Part	:Streaks_Explode_Short.part
		LifeSpan	5
	End
End

Condition
	On 	Time
	Time 	31

	Event
		Type	start
		At	GunOffset
		AltPiv	1
		Part	:Rays_Explode.part
		Part	:BlindingLightRays.part
		Part	:Shockwave.part
		LifeSpan	5
	End
End



#############################################################

End


