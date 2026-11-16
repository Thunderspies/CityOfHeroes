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
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmR
		LifeSpan	20
		Sound DarkBlast1 80 80 .77

	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	20
	End
	
End


Condition
	On 	Time
	Time 	22

	
	Event
		EName 	Prime
		Type	Start 
		At	WepR
		
		Part2	:BeamRapidTenticles.part
		Part1	:DarkBlastPuddleTrail.Part
		Part3	:BeamRapidCloud2.part
		Part4	:MoonBeamEmber.part
		Part5	:MoonBeamEmberLarge.part
		
		Part1	:MoonBeamRapidCloud.part
		Part2	:BeamRapidTenticlesHead.part
		Part3	:DarkBlastTendrill01.part
			
		Part4	:DarkBlastTendrill03.part
		
		Part2	:DarkBlastTendrill02.part
		Part3	:DarkBlastTendrill04.part
		

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
		Part1	:Blast.part
		Part2	:Blast.part
		Part3	:BlackStarCore.part
		Part4	:CreateDarknessPuddle2.part
		Lifespan	13
						
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
		Part1	:DarknessHitRing1.part
		Part1	:DarknessHitTendrills.part
		

		LifeSpan	15
		
	End

	Event
		EName 	Rings
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Part1	:BlackStarRing.Part
		Part2	:DarkBlastPuddleExplosion.Part
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


			