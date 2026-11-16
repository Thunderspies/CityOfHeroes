#############################################################
## ChockingSulphurCloudsHit.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Head
		part	Powers\EarthControl\ChockingCloudDotsHitx2.part
		Part	Powers\EarthControl\ChockingCloudDotsHitx.part
		Part	Powers\EarthControl\ChockingCloudBubblesHitx.part
		Part	Powers\EarthControl\ChockingCloudSmoke.part
		Part	Powers\EarthControl\ChockingCloudMist.part
		PMagnet	Head
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Head
		part	Powers\EarthControl\ChockingCloudSmokeTrail2.part
		Part	Powers\EarthControl\ChockingCloudSmokeTrail.part
		Part	Powers\EarthControl\ChockingCloudMistTrail.part
		PMagnet	Head
	End
End

#############################################################

End