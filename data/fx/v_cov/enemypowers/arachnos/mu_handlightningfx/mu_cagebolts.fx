#############################################################
## mu_CageBolts.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	origin
		Sound mu_cageboltramp 120 120 .9
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	origin
		Sound mu_cagebolts 120 120 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Ename	HandR
		Type	local
		At	wepL
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs.part
		Lifespan  15
	End

	Event
		Ename	HandL
		Type	local
		At	wepR
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs.part
		Lifespan  15
	End

	Event
		Ename	HandR
		Type	local
		At	wepL
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs_White.part
		Lifespan  25
	End

	Event
		Ename	HandL
		Type	local
		At	wepR
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs_White.part
		Lifespan  25
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandL
		Part1	V_COV\EnemyPowers\Arachnos\mu_CageRing1.part
		Lifespan 5
	End

	Event
		Type	start
		At	HandR
		Part1	V_COV\EnemyPowers\Arachnos\mu_CageRing1.part
		Lifespan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	bar
		Type	positonly
		At	T_Root
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBar1.Bhvr
	End

	Event
		Ename	bar1
		Type	local
		At	bar
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBar1.Bhvr
	End

	Event
		Ename	baroff1
		Type	local
		At	bar1
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBarOff1.Bhvr
	End

	Event
		Ename	baroff1N
		Type	local
		At	bar1
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBarOffNeg1.Bhvr
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Ename	lift
		Type	local
		At	chest
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageLift.Bhvr
		Magnet	baroff1N
		Lifespan 35
	End

	Event
		Ename	spotR
		Type	local
		At	chest
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerR.Bhvr
		Magnet	baroff1N
		Lifespan 35
	End

	Event
		Ename	spotL
		Type	local
		At	chest
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerL.Bhvr
		Magnet	baroff1
		Lifespan 35
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Ename	Prime
		Type	start
		At	spotR
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.Bhvr
		Magnet	baroff1N
		Lifespan 35
	End

	Event
		Type	local
		At	Prime
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock2.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShockSpark.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShockSpark_White.part
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Ename	trackerL
		Type	start
		At	spotL
		Bhvr	V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.Bhvr
		Magnet	baroff1
		Lifespan 35
	End

	Event
		Type	local
		At	trackerL
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShock2.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShockSpark.part
		part1	V_COV\EnemyPowers\Arachnos\mu_CageShockSpark_White.part
		Lifespan 35
	End
End

#############################################################

Condition	#### Lightning arcs to "activate" the cage
	On 	Time
	Time 	10

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	neck
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	chest
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotR
		Lifespan 6
	End

	Event
		Type	local
		At	hips
		part1	V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
		POther	spotL
		Lifespan 6
	End
End

#############################################################

Condition  #######explosion on trackers when the attack needs to hit
	On	PrimeHit

	Event
		Type	local
		At	baroff1
		part1	V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst1.part
		part2	V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst2.part
		part3	V_COV\EnemyPowers\Arachnos\mu_CageFence3.part
		Lifespan 15
	End

	Event
		Type	local
		At	baroff1N
		part1	V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst1.part
		part2	V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst2.part
		part3	V_COV\EnemyPowers\Arachnos\mu_CageFence3.part
		Lifespan 15
	End
End

#############################################################

End