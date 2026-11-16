
FxInfo
Flags		InheritAnimScale

#####  Test Node   #######################################

Condition
	On 	Time
	Time 	0
	#Flag	InheritAnimScale

	Event
		EName	TestNode					#existing event name, not totally clear
		Type	Local
		#At	Fore_LlegL
		#At	Fore_FootL
		#At	Fore_ToeL
		#At	Fore_LlegR
		#At	Fore_FootR
		#At	Fore_ToeR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		#At	Mystic
		#At	ToeL
		#At	Head
		#At	ToeR
		At	HandL
		#At	RingR
		#At	Hips
		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#Bhvr	V_COV\EnemyPowers\Arachnos\Crab_ArmOffset01.bhvr
		#Geom	TESTING_Target					#call to geometry fx
	End

End

Condition
	On 	Time
	Time	100

	Event
		EName 	TestNode
		Type	Destroy
	End



End