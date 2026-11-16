#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

######## Sound  ###########################

Condition
	On	Time
	Time	36

	Event	
		Type	Local
		At	WepR
		Sound thornytoss2 70 70 .7
	End
End

###############################################################################

Condition
	On	Time
	Time	33

	Event
		Ename	HandNode
		Type	start
		At	bendmystic
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
		AltPiv 1
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



######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	35

	
	Event
		EName 	prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		#Geom	SpineProjectile

		Magnet	target
		Lookat	target
		#LifeSpan 8

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
		#Sound 	bowtwangcone2 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	1
		Lookat	HandNode
		LifeSpan 17

	End
End


Condition
	On 	Time
	Time 	35

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
		#Sound 	bowtwangcone2 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

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
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	1
		Lookat	HandNode
		LifeSpan 17

	End
End


Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

End

End		