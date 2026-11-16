#########################################################
##	
##

FxInfo
LifeSpan  180
##################################


Condition
	On	Time
	Time	41

	Event
		Type	Local	
		At	Origin
		Sound tentacleattack2 80 80 .52
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local	
		At	Origin
		Sound demonboxopen 80 80 .8
	End
End



Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime1
		Type	local 
		At	wepR
	End


	Event
		EName 	Box
		Type	local 
		At	Prime1
		geom	box_ornate_base
		bhvr	:propFade.bhvr
		bhvrOverride
			Rgb0		255 255 255
			PositionOffset	.5 .1 0
			pyrRotate	180 -90 0 #1.5708
		End
		lifespan	70
	End

	Event
		EName 	BoxLid
		Type	local 
		At	Box
		geom	box_ornate_Lid
		bhvr	:propFade.bhvr
		bhvrOverride
			Rgb0		255 255 255
			Scale		1 1 1
			PositionOffset	0 .4 -.407
			Spin		.025 0 0
		End
		lifespan	70
	End

	Event
		EName 	Glow
		Type	Local
		At	Box	 
		Part	:DemonBoxGlow.part
		Part	:DemonBoxShine.part
		bhvrOverride
			PositionOffset	0 .5 0
		End
		lifespan	50
	End

	Event
		EName 	Sigil
		Type	Local
		At	Box	 
		Part	:DemonBoxSigil.part
		bhvrOverride
			PositionOffset	0 .5 0
		End
		lifespan	70
	End

	Event
		EName 	DarkMatter
		Type	Local 
		At	Box
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part2	POWERS/DarknessControl/DarkHandsB.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
				
		PMagnet	BoxLid
		LifeSpan	48
	End

End

Condition
	On 	Time
	Time 	41

	Event
		EName 	Prime
		Type	Start 
		At	Box
		
		bhvr	V_COV\WEAPONFX/DynamiteProjectile.bhvr
		bhvrOverride
			TrackRate	2
		End
		
		Part5	POWERS/DarknessControl/TendrilTenticlesColor.part
		Part1	POWERS/DarknessControl/TendrilTenticles.part
		
		Part2	POWERS/DarknessControl/TendrillClouds2.part
		Part3	POWERS/DarknessControl/TendrillClouds.part
		
		Magnet	T_Root
		LookAt	T_Root
				
	End
End

##################################

Condition
	On 	PrimeHit

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part2	V_COV\WEAPONFX/DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Flash
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part	
		Sound Demonbox 80 80 .76
		Lifespan 5
		
	End

	
	Event
		EName 	Mists
		Type	start
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:RedMist01.part
		Part	:RedMist02.part
		Part	:RedMistLight.part
		Part	:RedMistDark.part
		Part1	:RedMistFlat.part
		LifeSpan	45
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


##################################


End							