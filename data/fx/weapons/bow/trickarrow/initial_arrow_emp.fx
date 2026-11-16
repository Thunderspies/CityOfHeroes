#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 300


#### arrow in hand  ########################

Condition
	On	Time
	Time	33

	Event	
		Type	Local
		At	WepR
		Sound arrowoutEMP 50 50 .33
	End
End



Condition
	On 	Time
	Time 	35

	Event
		EName 	arrow
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateIce.bhvr
		Geom	EMPArrow
		LifeSpan 46
	End
	
	Event
		EName 	point1a
		Type	Local
		At	arrow
		altpiv	1
		#BHVR	:EMPOffset1.bhvr
		Part	:EMPArch1.part
		Part	:EMPArch2.part
		Part	:EMPglows.part
		
		Pmagnet	point2
		LifeSpan 46
	End

	Event
		EName 	point2a
		Type	Local
		At	arrow
		altpiv	2
		#BHVR	:EMPOffset.bhvr
		Part	:EMPArch1.part
		Part	:EMPArch2.part
		Part	:EMPglows.part
		
		Pmagnet	point1
		LifeSpan 46
	End

End

Condition
	
	On 	cycle
	Time 	5
	Chance	.3

	Event
		EName 	b
		Type	Local
		At	arrow
		altpiv	1

		#BHVR	:EMPOffset1.bhvr
		Part	:EMPFlash.part
		Part	:EMPFlashRing.part
		Part	:EMPFlashStars.part
		
		LifeSpan	5
	End
	
End


Condition
	
	On 	cycle
	Time 	5
	Chance	.3

	Event
		EName 	c
		Type	Local
		At	arrow
		altpiv	2

		#BHVR	:EMPOffset.bhvr
		Part	:EMPFlash.part
		Part	:EMPFlashRing.part
		Part	:EMPFlashStars.part
		
		LifeSpan	5
	End
	
End

####  projectile arrow  ##############################

Condition
	On 	Time
	Time 	81

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Weapons/Bow/Archery/projectile.bhvr
		Geom	aEMPArrow
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	point1
		Type	Local
		At	Prime
		altpiv	1
		#BHVR	:EMPOffset1.bhvr
		Part	:EMPArch1.part
		Part	:EMPArch2.part
		Part	:EMPglows.part
		
		Pmagnet	point2
	End
	
	Event
		EName 	point2
		Type	Local
		At	Prime
		altpiv	2
		#BHVR	:EMPOffset.bhvr
		Part	:EMPArch1.part
		Part	:EMPArch2.part
		Part	:EMPglows.part
		
		Pmagnet	point1
	End

End

Condition
	
	On 	cycle
	Time 	5
	Chance	.3

	Event
		EName 	point3
		Type	Local
		At	Prime
		altpiv	1

		#BHVR	:EMPOffset1.bhvr
		Part	:EMPFlash.part
		Part	:EMPFlashRing.part
		Part	:EMPFlashStars.part
		
		LifeSpan	5
	End
	
End


Condition
	
	On 	cycle
	Time 	5
	Chance	.3

	Event
		EName 	point4
		Type	Local
		At	Prime
		altpiv	2

		#BHVR	:EMPOffset.bhvr
		Part	:EMPFlash.part
		Part	:EMPFlashRing.part
		Part	:EMPFlashStars.part
		
		LifeSpan	5
	End
	
End
	
Condition

	On 	PrimeHit


	Event
		EName 	Mallet1
		Type	local 
		At	T_Chest
		bhvr	:EMPBubbleScale.bhvr
		Geom	ElectricalMeleeBubble
		
		LifeSpan	5
		
	End

	Event
		Type	start
		At	T_Chest	
		ChildFx  WEAPONS\Bow\TrickArrow\EmpPulse.fx	
		lifeSpan	80
	End

#	Event
#		Type	start 
#		At	T_chest
#		sound	arrowhitEMP 80 70 .8
#		Part	:Dots.part
#		Lifespan	10
#	End
#	
#	
#	Event
#		Type	start 
#		At	T_chest
#		Part	:HitEmber.part
#		Part	:HitStar.part
#		#Part	:HitEmberFlat.part
#		#Part	:HitStarFlat.part
#		Part	:HitRing.part
#		Part	:HitRing2.part
#		Part	:HitGlows2.part
#	
#		#Sound	PunchHit2 100.0 30.0 .8
#	End
#
#	Event
#		EName 	Mallet2
#		Type	start 
#		At	T_chest
#	
#		Part	:EMPElectricityArch1.part
#		Part	:EMPElectricityArch2.part
#		LifeSpan	15
#	End
#
	Event
		Type	local
		At	T_chest
			
		BHVR	Weapons/Bow/Archery/ArrowHit.bhvr
		Geom	NormalArrowHit
		
		Lifespan	64


	End

	Event
		Ename 	arrow
		Type	Destroy
	End

	Event
		Ename 	point1a
		Type	Destroy
	End
	
	Event
		Ename 	point2a
		Type	Destroy
	End

	Event
		Ename 	pointb
		Type	Destroy
	End
	
	Event
		Ename 	pointc
		Type	Destroy
	End

	Event
		Ename 	prime
		Type	Destroy
	End

	Event
		Ename 	point1
		Type	Destroy
	End

	Event
		Ename 	point2
		Type	Destroy
	End

	Event
		Ename 	point3
		Type	Destroy
	End

	Event
		Ename 	point4
		Type	Destroy
	End


End

End			