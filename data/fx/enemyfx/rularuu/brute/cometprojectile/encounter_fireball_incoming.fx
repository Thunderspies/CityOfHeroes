#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	16


#Condition
#	Event
#		EName	InitialSwordFire
#		Type	Local 
#		At	Waist
#		
#		
#		Sound	Fireball1 100.0 30.0 .8
#		LifeSpan 14
#	End
#End

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Waist
		#Part1	:Flash.part
		Sound incoming1 300 300 .6
		Sound incoming2 300 300 .6
		Sound incoming3 300 300 .7

	End

	Event
		Type	posit
		At	Waist

		Part	:StreaksMagnet.part
		Part	:DarkStreaksBlueMagnet.part
		Part	:DarkStreaksRedMagnet.part
		Part	:DarkStreaksMagnet.part
	End


	Event
		EName 	Prime
		Type	Local
		At	Waist
		Bhvr	behaviors/projectile1.bhvr


		#Part	:DarkStreaks.part
		#Part	:CometRings.part
		#Part	:CometRingsThin.part

		Part	:DarkBallHead.part
		Part	:DarkBallTail.part
		Part	:DarkBallStreak.part
	
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Waist
		Bhvr	behaviors/projectile1.bhvr
	

		Part	:DarkStreaksBlue.part
		Part	:DarkStreaksRed.part
		#Part	:SparkTrail.part
		#Part	:SparkTrail2.part
		#Part	:BigSparkTrail.part
		#Part	:BigSparkTrail2.part
		#Part	:BlackSparkTrail.part
	End
End



End	


			