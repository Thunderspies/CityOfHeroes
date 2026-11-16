#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300



######## Random Static Node  ###########################

Condition
	On	Time
	Time	0

	Event
		Ename	StaticNode
		Type	Start  
		At	WepR
	End
End




######## Activation flash  ###########################

Condition
	On	Time
	Time	2

	Event
		Type	Start
		At	StaticNode
		part2	POWERS\RadiationControl\RadiationAuraGreenLocal.part
		part3	POWERS\RadiationControl\RadiationAuraYellowLocal.part
		part4	POWERS\RadiationControl\RadiationAuraWhiteLocal.part	
		PMagnet	Chest
		LifeSpan	10
	End
End



######## Sound  ###########################

Condition
	On	Time
	Time	16

	Event	
		Type	Local
		At	StaticNode
		Sound thornytoss2 70 70 .7
	End
End

###############################################################################

Condition
	On	Time
	Time	13

	Event
		Ename	HandNode
		Type	start
		At	StaticNode
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
	Time 	16

	
	Event
		EName 	prime
		Type	start
		At	StaticNode
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
	Time 	16

	Event
		EName 	p
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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
		Bhvr	:ThornTree_projectile.bhvr
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