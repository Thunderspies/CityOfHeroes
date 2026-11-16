#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	WepR
		Sound thornytoss2 70 70 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	13

	Event
		Ename	HandNode
		Type	start
		At	BendMystic
		geom	Centerdummy

		magnet	target
		lookat	target
	End

	Event
		Ename	projectileTarget
		Type	local
		At	HandNode
		Altpiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	1
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	2
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	3
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	4
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	5
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End

	Event
		Ename	6
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	:offset.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	16

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
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	projectileTarget
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	5
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	4
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	3
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	2
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	1
		Lookat	HandNode
		LifeSpan 17
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	p
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	6
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	5
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	4
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	3
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	2
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Geom	SmallSpineProjectile
		Geom	SmallSpineProjectile01
		Geom	SmallSpineProjectile02
		Geom	SmallSpineProjectile03
		Magnet	1
		Lookat	HandNode
		LifeSpan 17
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	Local
		At	chest
		Sound Darthit1 70 70 0.9
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End