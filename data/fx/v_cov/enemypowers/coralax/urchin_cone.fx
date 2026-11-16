#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

#####  Steve test (this is to see if BendMystic bone is working)  ##########################################################################

#Condition
#	On	Time
#	Time	18
#
#	Event
#		Ename	test_cone
#		Type	local
#		At	BendMystic
#		Geom	Cone_template
#		Lifespan	25			 
#	End
#
#
#End
#
######## Sound  ###########################

Condition
	On	Time
	Time	35

	Event	
		Type	Local
		At	WepR
		#Sound	arrowout2 50 40 .25
	End
End

###############################################################################

Condition
	On	Time
	Time	13

	Event
		Ename	HandNode
		Type	start
		At	BendMystic  ##wepL
		geom	Centerdummy
		
#		magnet	target
#		lookat	target
		 
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
		At	BendMystic  ##WepL
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin

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
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	bowtwangcone2 100.0 30.0 .7

		Magnet	6
		Lookat	HandNode
		LifeSpan 18

	End

	
	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	5
		Lookat	HandNode
		LifeSpan 18
	End
	
	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	4
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	3
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	2
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	1
		Lookat	HandNode
		LifeSpan 18

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
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		Sound Urchinthrow2 80.0 80.0 .8

		Magnet	6
		Lookat	HandNode
		LifeSpan 18

	End

	
	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	5
		Lookat	HandNode
		
		LifeSpan 18
	End
	
	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	4
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		Geom	SmallSpineProjectile03
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	3
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	2
		Lookat	HandNode
		LifeSpan 18

	End
	
	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	:projectile.bhvr
		Part	:UrchinStreaks01.part
		Geom	Corolax_Urchin
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	1
		Lookat	HandNode
		LifeSpan 18

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