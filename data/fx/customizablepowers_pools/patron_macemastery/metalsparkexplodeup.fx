#########################################################
##	template

FxInfo

#########################################################

Condition
	On Time
	Time 0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkExplodeUP.bhvr
		CThresh 	0.000001

		Part	CustomizablePowers_Pools\Epic_WeaponMastery\MetalSpark.part
		Part	CustomizablePowers_Pools\Epic_WeaponMastery\MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End

End

