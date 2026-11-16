#
FxInfo

	LifeSpan 300


###########################################################

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_UpThrust_Attack_01 200 100 .8
	End
End

	Condition
		On Time
		Time 25

		Event
			At FootR
			Type Start
			Part ENEMYFX\Praetorian\Talons\Earth_UpThrust_FootImpactDust.part
			#Geom GEO_FX_TestSphere
			LifeSpan 28
		End

	End

	Condition
		On Time
		Time 30

		Event
			At FootR
			Type Start
			EName Prime
			LookAt T_Root
			Magnet T_Root
			Bhvr CustomizeablePowers\BeamRifle\Tracer_Fade.bhvr
			BhvrOverride
				TrackRate 1.5
			End
			Part ENEMYFX\Praetorian\Talons\Earth_UpThrust_GroundTrail_Rocks.part
			Part ENEMYFX\Praetorian\Talons\Earth_UpThrust_GroundTrail.part
			Part ENEMYFX\Praetorian\Talons\Earth_UpThrust_GroundTrail_EnergyStreaks.part
			LifeSpan 300
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Root
			Type Local
			ChildFx ENEMYFX\Praetorian\Talons\EarthSibyl_UpThrust_Hit.fx
		End

	End

End
