#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200


Input  
	Inpname	WepL
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmL
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

Input  
	Inpname	Root
End

Input  
	Inpname	T_Root
End
#########################################################

Condition
	On 	Time
	Time 	11

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		#Part2	POWERS/DarknessControl/DarkHandsB.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
				
		PMagnet	LarmR
		Sound dctorrent 90 90 .85
		LifeSpan	25
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		#Part2	POWERS/DarknessControl/DarkHandsB.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
				
		PMagnet	LarmL
		LifeSpan	25
	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		#Part2	POWERS/DarknessControl/DarkHands2.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	25
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		#Part2	POWERS/DarknessControl/DarkHands2.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	25
	End

End


################  Start   #####################

Condition
	On	Time
	Time	5
	Event
		EName 	binder7 
		Type	local
		At	UarmR
		Bhvr	:soulBinderArm2.bhvr
		Lifespan	15
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


################   2   ##########################

Condition
	On	Time
	Time	5

	Event
		EName 	binder8 
		Type	local
		At	UarmL
		Bhvr	:soulBinderArm1.bhvr
		Lifespan	15
	End

	Event
		EName 	Offset8 
		Type	local
		At	binder8
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset8
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset8
		  AltPiv	4
		Part	:soulFire0.part
	End

End
################   3   ##########################

Condition
	On	Time
	Time	14
	Event
		EName 	binder9 
		Type	local
		At	larmR
		Bhvr	:soulBinderArm2.bhvr

		Lifespan	15
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


################   4   ##########################

Condition
	On	Time
	Time	14

	Event
		EName 	binder10 
		Type	local
		At	larmL
		Bhvr	:soulBinderArm1.bhvr
		
		Lifespan	15
	End

	Event
		EName 	Offset10 
		Type	local
		At	binder10
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset10
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset10
		  AltPiv	4
		Part	:soulFire0.part
	End

End

############   Hands   ###########


Condition
	On	Time
	Time	20
	
	Event
		EName 	HandR 
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	3
	End


	Event
		EName 	HandL 
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	3
	End

End



#############  Projectile   #########################

Condition
	On 	Time
	Time 	21

	Event
		EName 	p
		Type	Start 
		At	Root
		Geom	Torrent				
	End

	Event
		EName 	Prime
		Type	Start 
		At	p
		 Altpiv	1
		Bhvr	behaviors/MedianProjectile4.bhvr
		
		Part5	POWERS/DarknessControl/TendrilTenticlesColor.part
		Part1	POWERS/DarknessControl/TendrilTenticles.part
		
		Part2	POWERS/DarknessControl/TendrillClouds2.part
		Part3	POWERS/DarknessControl/TendrillClouds.part

		Part	:soulCloud2.part
		Part	:soulGlow1.part
		#Part	:soulGlow0.part
		
		Magnet	T_Root
		LookAt	T_Root
				
	End
End

	

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			