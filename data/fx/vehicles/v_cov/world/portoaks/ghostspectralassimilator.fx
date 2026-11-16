FxInfo

LifeSpan	150

#################################################################################

Condition
	
	Event
		Ename	Prime
		Type	local
		At	Target

		Part	:SuckingGhostSpawnCloudsFlat4.part
		Part	:SuckingGhostSpawnCloudsFlat3.part
		Part	:SuckingGhostSpawnCloudsFlat2.part
		Part	:SuckingGhostSpawnCloudsFlat.part
		Part	:SuckingGhostSpawnRays.part

		PMagnet	Origin
		Magnet  Target
		#Geom	FX_ICEWEAPON
		PMagnet	Target

	End

	Event
		Ename	Prime
		Type	start
		At		Origin

		Part	:SuckingGhostSpawnCloudsFlat.part
		Part	:SuckingGhostSpawnRays.part
		LookAt   Target
		Magnet  Target
		#Geom	FX_ICEWEAPON
		PMagnet	Target
	

	End

End

End