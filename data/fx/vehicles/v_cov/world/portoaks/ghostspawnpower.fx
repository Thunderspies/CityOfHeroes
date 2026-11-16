FxInfo

LifeSpan	240

#################################################################################

Condition
	
	Event
		Ename	Prime
		Type	start
		At	t_Root
		Bhvr	:GhostPowerLobbprojectile.bhvr
		Part	:Poultergeists.part
		Part	:PoultergeistsGLOW2.part

		Magnet	root
		Lookat	root
	End

	Event
		Type	start
		At	t_chest
		#Bhvr	:GhostPowerLobbprojectile.bhvr
		Part	:HitRingPoultergeists.part
		LifeSpan	20
	End
	
	Event
		Type	start
		At	t_chest
		#Bhvr	:GhostPowerLobbprojectile.bhvr
		Part	:HitRingPoultergeists2.part


		Part	:SuckingGhostSpawnCloudsFlat4.part
		Part	:SuckingGhostSpawnCloudsFlat3.part
		Part	:SuckingGhostSpawnCloudsFlat2.part
		Part	:SuckingGhostSpawnCloudsFlat.part
		Part	:SuckingGhostSpawnRays.part


		LifeSpan	5
	End

End

Condition
	On	PrimeHit
	
	Event
		Type	start
		At	Root
		
		Bhvr	Behaviors/RootToChestAdjustment.bhvr
		Part	:HitRingPoultergeists.part
		LifeSpan	30

	End

	Event
		Type	start
		At	Root
		
		Bhvr	Behaviors/RootToChestAdjustment.bhvr
		Part	:HitRingPoultergeists2.part

#		
#		Part	:SuckingGhostSpawnCloudsFlat4.part
#		Part	:SuckingGhostSpawnCloudsFlat3.part
#		Part	:SuckingGhostSpawnCloudsFlat2.part
#		Part	:SuckingGhostSpawnCloudsFlat.part
#		Part	:SuckingGhostSpawnRays.part
#
		LifeSpan	15

	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End