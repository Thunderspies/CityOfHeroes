#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

Input  
	InpName	Origin
End

Input  
	InpName	Target
End
 
Input  
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End

Input  
	Inpname	WepR
End

#########################################################
Condition

	On	Time
	Time	8
	Event
		Type	Local
		At	origin
		Sound widowcast4 80 80 .96
	End

End

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
		Sound DarkBlast1 80 80 .77

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


##############  Projectile  ###################

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
		

		Bhvr	behaviors/slowprjct5.bhvr
	
		Part1	POWERS/DarknessControl/MoonBeamRapidCloud.part
		#Part2	POWERS/DarknessControl/BeamRapidTenticlesHead.part
		Part3	POWERS/DarknessControl/DarkBlastTendrill01.part
			
		Part4	POWERS/DarknessControl/DarkBlastTendrill03.part
				

		Magnet	Target
		LookAt	Target
		
		Part2	POWERS/DarknessControl/DarkBlastTendrill02.part
		Part3	POWERS/DarknessControl/DarkBlastTendrill04.part

		
		Part	:soulSmoke1.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part
						
	End


End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local 
		At	target
		Part1	POWERS/DarknessControl/Blast.part
		Part2	POWERS/DarknessControl/Blast.part
						
	End
	
	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	Prime3
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


			