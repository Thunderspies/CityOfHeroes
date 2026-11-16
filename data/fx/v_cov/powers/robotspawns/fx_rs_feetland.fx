
FxInfo

## hit ground ##

Condition
	On	Time
	Time	30

	Event
		EName	FootRSound				# first foot impacting ground
		Type	Local
		At	FootR
		bhvr	behaviors\FX_RobotSpawn_CamShake.bhvr
		Sound giantfoot1 85 85 .68
	End

End

Condition
	On	Time
	Time	33

	Event
		EName	FootLSound				# second foot impacting ground
		Type	Local
		At	FootL
		bhvr	behaviors\FX_RobotSpawn_CamShake01.bhvr
		Sound giantfoot1 85 85 .68
	End

End

## Feet Hit ##

Condition
	On 	Time
	Time 	30
	Event
		EName	Ring10
		Type	Local
		At	FootR
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRingEnd.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDustEnd01.part
		LifeSpan	1		
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName	Ring10
		Type	Local
		At	FootL
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRingEnd.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDustEnd01.part
		LifeSpan	1		
	End

End

End



