#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

ClampMaxScale 1 1 1

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	arrow
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotateIce.bhvr
		Geom	EMPArrow
		Sound arrowoutEMP 50 50 .33
		LifeSpan 94
	End

	Event
		Type	Local
		At	arrow
		altpiv	1
		Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
		Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
		Part	WEAPONS\Bow\TrickArrow\EMPglows.part
		Pmagnet	point2
		LifeSpan 94
	End

	Event
		Type	Local
		At	arrow
		altpiv	2
		Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
		Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
		Part	WEAPONS\Bow\TrickArrow\EMPglows.part
		Pmagnet	point1
		LifeSpan 94
	End

	Event
		Type	Local
		At	arrow
		altpiv	1
		ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
		LifeSpan	94
	End

	Event
		Type	Local
		At	arrow
		altpiv	2
		ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
		LifeSpan	94
	End
End

#Condition
#	On 	Time
#	Time 	48
#
#	Event
#		EName 	Prime
#		Type	start
#		At	WepL
#		Bhvr	Weapons\Bow\Archery\projectile.bhvr
#		Geom	aEMPArrow
#		Sound Crossbow1 100.0 100.0 .7
#		Magnet	SpreadAnchor
#		LookAt	SpreadAnchor
#	End
#
#	Event
#		EName 	point1
#		Type	Local
#		At	Prime
#		altpiv	1
#		Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
#		Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
#		Part	WEAPONS\Bow\TrickArrow\EMPglows.part
#		Pmagnet	point2
#	End
#
#	Event
#		EName 	point2
#		Type	Local
#		At	Prime
#		altpiv	2
#		Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
#		Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
#		Part	WEAPONS\Bow\TrickArrow\EMPglows.part
#		Pmagnet	point1
#	End
#
#	Event
#		EName 	point3
#		Type	Local
#		At	Prime
#		altpiv	1
#		ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
#	End
#
#	Event
#		EName 	point4
#		Type	Local
#		At	Prime
#		altpiv	2
#		ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
#	End
#End
#
##############################################################
#
#Condition
#	On 	PrimeHit
#
#	Event
#		EName 	Mallet1
#		Type	PositOnly
#		At	Target
#		bhvr	WEAPONS\Bow\TrickArrow\EMPBubbleScale.bhvr
#		Geom	ElectricalMeleeBubble
#		LifeSpan 5
#	End
#
#	Event
#		Type	PositOnly
#		At	Target
#		ChildFx	WEAPONS\Bow\TrickArrow\EmpPulse.fx
#		LifeSpan 80
#	End
#
#	Event
#		Ename 	prime
#		Type	Destroy
#	End
#
#	Event
#		Ename 	point1
#		Type	Destroy
#	End
#
#	Event
#		Ename 	point2
#		Type	Destroy
#	End
#
#	Event
#		Ename 	point3
#		Type	Destroy
#	End
#
#	Event
#		Ename 	point4
#		Type	Destroy
#	End
#End

#############################################################

End