FxInfo

#LifeSpan	240

#################################################################################

Condition

	On 	Time
	Time 	10
	Chance	1
	
	Event
		Ename	Prime
		Type	start
		At	root

		BHVR	:Projectile.bhvr
		Part	:GhostSpawnCloudsFlat4a.part
		Part	:GhostSpawnCloudsFlat3a.part
		Part	:GhostSpawnCloudsFlat2a.part
		Part	:GhostSpawnCloudsFlata.part
		Part	:GhostSpawnRaysFlat.part
	
		Magnet	t_Root
		LifeSpan	30
	End


End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

End