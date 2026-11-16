
FXInfo

Lifespan 100

Condition
	On 	Time
	Time 	0

	Event
		EName	Webbing
		Type	Start
		At	Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapSpin.bhvr
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCloudBurst01.part
		Sound webcagehit 88 88 .95
		Magnet	Root
		LookAt	Root
		Lifespan 30
	End
End

#Condition
#	On	Time
#	Time	0	
#
#	Event
#		EName	Cage
#		Type	Local
#		At	Root
#
#		ChildFX	V_COV\EnemyPowers\Arachnos\Arachnoid_WebTrapCage.fx
#		lifespan 130
#	End
#End
#
End