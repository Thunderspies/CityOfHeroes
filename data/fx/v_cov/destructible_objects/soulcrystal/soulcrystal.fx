#############################
###   Soul Crystal Glow   ###
#############################

FxInfo

#############################

Condition
	On	Time
	Time	0

	Event	

		ENAME	Base
		Type	local
		At	Root

		Part1	V_COV\EnemyPowers\GhostWidow\TendrilPuddle.part
		Part2	V_COV\EnemyPowers\GhostWidow\TendrilPuddle2.part
	End

	Event
		EName 	glow 
		Type	local
		At	Base
		bhvr	behaviors/genericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 5 0
		End
		Part	V_COV\EnemyPowers\GhostWidow\soulChoke1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSuck1.part
		part	:CrystalGlow02.part
		part	:CrystalGlow03.part
	End

	Event
		Ename	Pulsing
		Type	Local
		At	Base
		Pmagnet	glow
		part	:CrystalGlow01.part
	End

End


################   P1   ##########################

Condition
	On	Time
	Time	3


	Event
		EName 	spirit1
		Type	local
		At	Base
		Bhvr	V_COV\EnemyPowers\GhostWidow\Spinner01.bhvr
		BhvrOverride
			PositionOffset	0 2.5 0
			#pyrRotate	0 1.5708 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End


	Event
		EName 	orbit1 
		Type	local
		At	spirit1
		BhvrOverride
			PositionOffset	0 0 1
		End
		
	End


	Event
		EName 	child
		Type	Local
		At	orbit1
		Bhvr	V_COV\EnemyPowers\GhostWidow\spinner02.bhvr
	End

	Event
		EName 	Offset1 
		Type	local
		At	child
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulOffset.bhvr
		BhvrOverride
			PositionOffset	0 0 1
		End
#		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset1
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset1
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End


End


#######################  P2  ####################

Condition
	On	Time
	Time	6


	Event
		EName 	spirit2
		Type	local
		At	Base
		Bhvr	V_COV\EnemyPowers\GhostWidow\Spinner01.bhvr
		BhvrOverride
			PositionOffset	0 4.25 0
			pyrRotate	0 90 0
			#pyrRotateJitter	.2 0 .2
		End
		
	End


	Event
		EName 	orbit2 
		Type	local
		At	spirit2
		BhvrOverride
			PositionOffset	0 0 1
		End
	End


	Event
		EName 	child2 
		Type	Local
		At	orbit2
		Bhvr	V_COV\EnemyPowers\GhostWidow\spinner02.bhvr
	End

	Event
		EName 	Offset2 
		Type	local
		At	child2
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulOffset3.bhvr
		BhvrOverride
			PositionOffset	0 0 1
		End
#		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset2
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset2
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End


End


######################## P3 ############################


Condition
	On	Time
	Time	9


	Event
		EName 	spirit3
		Type	local
		At	Base
		Bhvr	V_COV\EnemyPowers\GhostWidow\Spinner01.bhvr
		BhvrOverride
			PositionOffset	0 6 0
			pyrRotate	0 180 0
			#pyrRotateJitter	.2 0 .2
		End
		
	End


	Event
		EName 	orbit3 
		Type	local
		At	spirit3
		BhvrOverride
			PositionOffset	0 0 1
		End
	End


	Event
		EName 	child3 
		Type	Local
		At	orbit3
		Bhvr	V_COV\EnemyPowers\GhostWidow\spinner02.bhvr
	End

	Event
		EName 	Offset3 
		Type	local
		At	child3
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulOffset3.bhvr
		BhvrOverride
			PositionOffset	0 0 1
		End
#		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset3
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset3
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

End

#######################   P4   ################################

Condition
	On	Time
	Time	12


	Event
		EName 	spirit4
		Type	local
		At	Base
		Bhvr	V_COV\EnemyPowers\GhostWidow\Spinner01.bhvr
		BhvrOverride
			PositionOffset	0 7.75 0
			pyrRotate	0 -90 0
			#pyrRotateJitter	.2 0 .2
		End
		
	End


	Event
		EName 	orbit4 
		Type	local
		At	spirit4
		BhvrOverride
			PositionOffset	0 0 1
		End
	End


	Event
		EName 	child4 
		Type	Local
		At	orbit4
		Bhvr	V_COV\EnemyPowers\GhostWidow\spinner02.bhvr
	End

	Event
		EName 	Offset4 
		Type	local
		At	child4
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulOffset.bhvr
		BhvrOverride
			PositionOffset	0 0 1
		End
#		Part	V_COV\EnemyPowers\GhostWidow\soulGlow1.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset4
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset4
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire1.part
	End

End


End