#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Mystic
End

LifeSpan	200


Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Start 
		At	Mystic
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	powers/firecontrol/FireBallHead.part
		Part2	powers/firecontrol/FireBallTail.part
		Part3	powers/firecontrol/FireBallStreak.part
		Part4	:SparkTrail.part
		Sound fireball1 80 80 .7
	End


End


Condition
	On 	primehit
	Repeat  11

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
	
#	Event 
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	T_hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#
#	Event 
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	T_Chest
#		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End


Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target

		Part	:BigFireExplosion.part
		Part	:FireBallHitSpark.part
	End
#
	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	T_Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
#
	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			