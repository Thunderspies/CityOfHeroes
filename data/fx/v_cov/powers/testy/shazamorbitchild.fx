#########################################################
##	Shazam Orbit Child
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Start 
		At	wepL
		Bhvr	:ShazamChildprojectile01.bhvr
		Magnet	Target
		#LookAt	Target
		Part	:FireBallHead.part
		Part	:ShazamSpinner.part
		Part	:FireBallStreak.part
		Part	:ShazamSparkTrail.part
	End
End

Condition
	On	Prime1Hit
			
	Event
		Ename	OrbiterHit
		Type	local
		At	target

		Part	:BigFireExplosion.part
		Part	:ShazamImpact.part
		Part	:ShazamFlash.part
		Part	:FireBallHitSpark.part
	End

	Event
		Ename	Prime1
		Type	Destroy
	End

End

End	


			