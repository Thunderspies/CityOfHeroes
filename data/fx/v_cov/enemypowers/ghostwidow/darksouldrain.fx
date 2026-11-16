#############################################################
## DarkSoulDrain.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	origin
		Sound widowspincast3 70 70 .7
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		Sound SoulDrain2 80 80 .7
		PMagnet	LarmL
		LifeSpan 50
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		PMagnet	WepL
		LifeSpan 42
	End
End

################  Start   #####################

Condition
	On	Time
	Time	10

	Event
		EName 	binder8
		Type	local
		At	UarmL
		Bhvr	:soulBinderArm1.bhvr
		BhvrOverride
			PositionOffset -.2 0 0
		End
		Lifespan	20
	End

	Event
		EName 	Offset8
		Type	local
		At	binder8
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset8
		AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset8
		AltPiv	4
		Part	:soulFire0.part
	End
End

Condition
	On	Time
	Time	16

	Event
		EName 	binder10
		Type	local
		At	larmL
		Bhvr	:soulBinderArm1.bhvr
		Lifespan	20
	End

	Event
		EName 	Offset10
		Type	local
		At	binder10
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset10
		AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset10
		AltPiv	4
		Part	:soulFire0.part
	End
End

Condition
	On	Time
	Time	22

	Event
		EName 	binder2
		Type	local
		At	WepL
		Bhvr	:soulBinderArm1.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
			InitialVelocityJitter	0 0 0
			SpinJitter	0.05 0.015 0.015
		End
		Lifespan	25
	End

	Event
		EName 	Offset2
		Type	local
		At	binder2
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset2
		AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset2
		AltPiv	4
		Part	:soulFire0.part
	End
End


Condition
	On	Time
	Time	22

	Event
		EName 	HandL
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke0.part
		Lifespan 23
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	WepL
		Bhvr	behaviors\Skullprojectile1.bhvr
		Geom	Skull
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	Prime2
		Type	local
		At	Prime
		altpiv	2
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End

	Event
		EName	Prime2
		Type	local
		At	Prime
		altpiv	3
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End

	Event
		EName	Prime2
		Type	start
		At	Prime
		altpiv	1
		Bhvr	behaviors\Skullprojectile1.bhvr
		Part2	POWERS\DarknessControl\DarknessRapidCloud.part
		Part3	POWERS\DarknessControl\DarknessRapidCloud2.part
		Part4	POWERS\DarknessControl\SoulDrainSkullGlow.part
		Part	:soulSmoke1.part
		Part	:soulGlow0.part
		Magnet	Target
		LookAt  Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	local
		At	T_Chest
		Part1	POWERS\DarknessControl\SkullHit.part
		Part2	POWERS\DarknessControl\SkullPuddle.Part
		Sound headcrunch 70 70 .98
		LifeSpan 25
	End

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End
End

#############################################################

End