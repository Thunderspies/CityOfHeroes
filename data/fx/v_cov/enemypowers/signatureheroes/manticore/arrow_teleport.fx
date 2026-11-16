#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300

######## arrow in hand   ###########################


Condition
	On	Time
	Time	10

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End



Condition
	On 	Time
	Time 	10

	Event
		Ename	knock
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 31
	End
		
	Event	
		ENAME	HeadNode
		Type	Local
		At	knock
		BhvrOverride
			PositionOffset	0 0 3.75
		End
		
	End
		
	Event	
		ENAME	HeadGlow
		Type	Local
		At	HeadNode
		part1	:TSHeadGlow01.part
		Pmagnet HeadNode
		Bhvr	:TelportationFade.bhvr
		Lifespan 0
		
	End
	
End

######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	41

	
	Event
		Ename 	HeadNode
		Type	Destroy
	End


	Event
		EName 	Prime1
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		Sound bowtwang3 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

	Event	
		ENAME	GlowFlash
		Type	Local
		At	Prime1
		part1	:TSWarpGlow01.part
		part1	:TSWarpFlash01.part
		Pmagnet Prime1
		Lifespan 10
	End

	Event
		EName 	Offset
		Type	start
		At	Target
		Bhvr	:projectile.bhvr

		Magnet	WepR
		LookAt	WepR
	End

End

########  arrow teleport OUT  ##########################

Condition

	On 	Time	
	Time	43

	Event
		Type	Local
		At	Prime1
		Sound teletrans3 80 80 .78
	End
		
	Event	
		ENAME	ArrowGlow
		Type	Start
		At	Prime1
		part1	:TSHeadGlow01.part
		part1	:TSHeadGlow01.part
		Pmagnet Prime1
		Bhvr	:TelportationFade.bhvr
		Lifespan 5
		
	End
	
	Event
		Ename 	Prime1
		Type	Destroy
	End

End

###########  Teleport IN  ##############

Condition
	On 	Time
	Time 	44

	Event
		EName 	Prime
		Type	start
		At	Offset
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow

		Magnet	Target
		LookAt	Target
	End

	Event	
		ENAME	GlowFlash
		Type	Start
		At	Prime
		part1	:TSWarpGlow01.part
		part1	:TSWarpFlash01.part
		part1	:TSHeadGlow01.part
		part1	:TSHeadGlow01.part
		Pmagnet GlowFlash
		Lifespan 10
	End

	
	Event
		Ename 	Offset
		Type	Destroy
	End
End

#############################################

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

End

End

		