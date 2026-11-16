#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	HandL
End

Input  
	Inpname	T_Root
End

LifeSpan	200

########################################################


Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound Mu_elecball 80 80 .93
	End
End


Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime
		Type	start
		At	HandL
		Part1	:mu_ball_Control01a.part
		Part2	:mu_ball_Control02a.part
		Part3	:mu_ball_InnerGlowa.part
		Part5	:mu_ball_BallTail.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
	
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime2
		Type	start
		At	HandL
		Part1	:mu_ball_OuterGlowa.part
		Part2	:mu_ball_Archa.part
		Part3	:mu_ball_Ringa.part
		Part4	:mu_ball_ArchInverta.part
		Part5	:mu_ball_BallTail2.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End

	Event
		
		Type	Local
		At	Target
		
		Part1	:mu_ball_Archsa.part
		Part2	:mu_ball_Archs2a.part
		Part3	:mu_ball_Archs1a.part
		Sound elecballhit 60 60 .6
		lifespan	30
	End

	Event
		ENAME	H
		Type	start
		At	T_Root
		
		
		geom	roottochestadjustment
		lifespan	30
	End
	
	Event
		EName 	Mallet
		Type	start 
		At	H
		altpiv	1

		Geom	ElectricityHit
		bhvr	behaviors/ScaleBubble2.bhvr
		lifespan	3.5
	End

	Event
		
		Type	start
		At	H
		altpiv	1
		Part1	:mu_ball_SparkBurst.part
		Part2	:mu_ball_SparkBurst.part
		Part4	:mu_ball_RingA.part
		lifespan	30
	End

	Event
		
		Type	Local
		At	Target
		Part2	:mu_ball_Glow.part
		
		lifespan	5
	End
End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End

End





#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:mu_ball_Control01.part
		Part2	:mu_ball_Control02.part
		Part3	:mu_ball_InnerGlow.part
		
	End


	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:mu_ball_OuterGlow.part
		Part2	:mu_ball_Arch.part
		Part3	:mu_ball_Ring.part
		
	End
End

Condition
	On 	Time
	Time 	25
	
	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:mu_ball_SparkBurst.part
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	35
	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

End

End