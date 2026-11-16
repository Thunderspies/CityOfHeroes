#########################################################
##	onfire
##
FxInfo  

LifeSpan 100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	origin
		Part1	explosions/large/Mediumfire_flame_A2.part
#		Part2	explosions/large/Mediumfire_flame_B.part
		
		Part3	explosions/large/Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
#		Part5	explosions/large/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
		Lifespan 30
	End

	Event
		EName 	Prime
		Type	start 
		At	origin
		ChildFX 	V_COV\World\Vault\FireballBurnHitDecal.fx
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	behaviors/SmallBurst.bhvr
		Part1	:FireHands2Small.part
		#Part2	:FireBoltTail2Small.part
		#Part3	:FireBoltTailSmall.part
		#Part4	:SparkTrailSmall.part
		Part5	:FireHands2Small.part
		Lifespan	300
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest

		Part1	:fireweaponexplosion.part
		Lifespan	10
	End

End

End			
