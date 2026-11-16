#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
				
		PMagnet	LarmR
		LifeSpan	20
		#Sound	DarkBlast1 80 33 .77
		Sound widowcast5 80 80 .96

	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	20
	End
	
End

################  Start   #####################

Condition
	On	Time
	Time	3
	Event
		EName 	binder7 
		Type	local
		At	UarmR
		Bhvr	:soulBinderArm2.bhvr
		Lifespan	20
	End

	Event
		EName 	Offset7 
		Type	local
		At	binder7
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset7
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset7
		  AltPiv	4
		Part	:soulFire0.part
	End

End

Condition
	On	Time
	Time	11
	Event
		EName 	binder9 
		Type	local
		At	larmR
		Bhvr	:soulBinderArm2.bhvr

		Lifespan	20
	End

	Event
		EName 	Offset9 
		Type	local
		At	binder9
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset9
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End


Condition
	On	Time
	Time	13
	
	Event
		EName 	HandR 
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	9
	End


End


########3  Projectile  ###################

Condition
	On 	Time
	Time 	22

	
	Event
		EName 	Prime
		Type	Start 
		At	WepR
		
		Part2	POWERS/DarknessControl/BeamRapidTenticles.part
		#Part1	POWERS/DarknessControl/DarkBlastPuddleTrail.Part
		#Part3	POWERS/DarknessControl/BeamRapidCloud2.part
		Part4	POWERS/DarknessControl/MoonBeamEmber.part
		Part5	POWERS/DarknessControl/MoonBeamEmberLarge.part
		
		Part1	POWERS/DarknessControl/MoonBeamRapidCloud.part
		#Part2	POWERS/DarknessControl/BeamRapidTenticlesHead.part
		Part3	POWERS/DarknessControl/DarkBlastTendrill01.part
			
		Part4	POWERS/DarknessControl/DarkBlastTendrill03.part
		
		Part2	POWERS/DarknessControl/DarkBlastTendrill02.part
		Part3	POWERS/DarknessControl/DarkBlastTendrill04.part
		
		
		Part	:soulSmoke1.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
						
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	T_chest
		#Bhvr	Behaviors/CameraShake05subtle.bhvr
		Part1	POWERS/DarknessControl/Blast.part
		Part2	POWERS/DarknessControl/Blast.part
		Part3	POWERS/DarknessControl/BlackStarCore.part
		Part4	POWERS/DarknessControl/CreateDarknessPuddle2.part
		Sound dirtexplo3 80 80 .8
		Lifespan	13
						
	End


	Event
		EName 	HitFlash
		Type	local 
		At	T_chest

		Part	:soulCloud2.part
		Part	:soulSparks1.part				
		Part	:soulSparks2.part
		Part	:soulFlash1.part
		Lifespan	3
						
	End

	Event
		EName 	GeomRing
		Type	local 
		At	T_Root
		Geom	RootToChestAdjustment
						
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	GeomRing
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	POWERS/DarknessControl/DarknessHitRing1.part
		Part1	POWERS/DarknessControl/DarknessHitTendrills.part
		

		LifeSpan	15
		
	End

	Event
		EName 	Rings
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Part1	POWERS/DarknessControl/BlackStarRing.Part
		Part2	POWERS/DarknessControl/DarkBlastPuddleExplosion.Part
		LifeSpan	20
	End

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Time
	Time	70

	Event
		EName 	HitFake
		Type	Destroy

	End
End

Condition
	On 	Time
	Time	50

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

	Event
		EName 	Hand3
		Type	Destroy

	End

	Event
		EName 	Hand4
		Type	Destroy

	End

End


End	


			