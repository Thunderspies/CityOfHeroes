#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

#############################################################

#Condition
#	On	Time
#	Time	15
#
#	Event
#		Type	Local
#		At	WepR
#		Sound	arrowout2 50 40 .25
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate1.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate2.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate3.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate4.bhvr
		Geom	NormalArrow
		LifeSpan 17
	End
End

#############################################################

Condition
	On	Time
	Time	27

	Event
		Ename	projectileTarget
		Type	local
		At	Bendmystic
		BHVR	:Scale.bhvr
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	1
		Type	local
		At	projectileTarget
		AltPiv 1
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	2
		Type	start
		At	projectileTarget
		AltPiv	2
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	3
		Type	start
		At	projectileTarget
		AltPiv	3
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	4
		Type	start
		At	ProjectileTarget
		AltPiv	4
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	5
		Type	start
		At	ProjectileTarget
		AltPiv	5
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

	Event
		Ename	6
		Type	start
		At	ProjectileTarget
		AltPiv	6
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End
End

#############################################################

Condition
	On 	Time
	Time 	27

	Event
		EName 	prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Magnet	target
		Lookat	target
	End

	Event
		EName 	p
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		Sound bowtwangcone2 100.0 100.0 .7
		Magnet	6
		Lookat	6
		LifeSpan 15
	End

	Event
		EName 	p2
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7
		Magnet	5
		Lookat	5
		LifeSpan 15
	End

	Event
		EName 	p3
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7
		Magnet	4
		Lookat	4
		LifeSpan 15
	End

	Event
		EName 	p4
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7
		Magnet	3
		Lookat	3
		LifeSpan 15
	End

	Event
		EName 	p2
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7
		Magnet	2
		Lookat	2
		LifeSpan 15
	End

	Event
		EName 	p1
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7
		Magnet	1
		Lookat	1
		LifeSpan 15
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

End

#############################################################

End