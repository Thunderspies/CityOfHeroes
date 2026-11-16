#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Hips
End

Lifespan	200

########################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	PrimeL
		Type	Local
		At	WepL
		Geom	GEO_WepR_Shuriken
		BhvrOverride
			Positionoffset		-.4 0 0
			Scale			2 2 2
		End
		Lifespan 5
	End

	Event
		EName	PrimeR
		Type	Local
		At	WepR
		Geom	GEO_WepR_Shuriken
		BhvrOverride
			Positionoffset		.4 0 0
			Scale			2 2 2
		End
		Lifespan 5
	End

	Event
		At	PrimeL
		Type	Local
		Ename	WeaponOffset
		Part	:Rings_Plasma.part
		PMagnet	WepL
		Lifespan 3
	End

	Event
		At	PrimeR
		Type	Local
		Ename	WeaponOffset2
		Part	:Rings_Plasma.part
		PMagnet	WepR
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	Anchor
		Type	StartPositOnly
		At	Chest
	End

	Event
		EName	Prime
		Type	start
		At	Anchor
		Geom	GEO_WepR_Shuriken
		#Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			Scale			3 3 3
			Spin			0 -1.5 0
			SpinJitter		0.1 0 0
			InitialVelocity		-0.8 0 0
			TrackRate		2.5
		End
		Part1	ENEMYFX\Tsoo\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Prime2
		Type	start
		At	Anchor
		Geom	GEO_WepR_Shuriken
		#Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			Scale			3 3 3
			Spin			0 -1.5 0
			SpinJitter		0.1 0 0
			InitialVelocity		0.8 0 0
			TrackRate		2.5
		End
		Part1	ENEMYFX\Tsoo\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End
End

End
##################################

