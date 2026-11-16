#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part	:FireHands.part
		#Bhvr	:testpivot.bhvr
		Sound Fireball1 100.0 100.0 .8
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:FireHandsFlash.part
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target

		BhvrOverride
		
			Hueshift	180
			HueshiftJitter	180
		
		End

		#Part	powers/firecontrol/FireBallHead.part
		
			####Adjustment for different looking Head####
			Part	powers/firecontrol/Test_FireBallHead.part

		Part	powers/firecontrol/FireBallTail.part
		Part	powers/firecontrol/FireBallStreak.part
		Part	:SparkTrail.part
	End
End

#Condition
#	On 	primehit
#	Repeat  11
#
#	Event 
#		EName 	Sparkz
#		Type 	Local
#		At 	T_Chest
#		####Physics####
#		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#End

Condition
	On 	primehit
#	Repeat  11

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		
		
		#ChildFx	Explosions\MetalExplosion\MetalExplosion.Fx


		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
#	Repeat  11

	Event 
#		EName 	Sparkz
		Type 	Local
		At 	T_hips
		
		
		####New hit fx####

		#ChildFx	Explosions\PortalExplosion\LargePortalExplosion.fx
		


		Lifespan 70
		LifespanJitter 	20
	End
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

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			