#########################################################
##	Mirror Projectile
########################################################
FxInfo

LifeSpan  300
############################################


Condition
	On 	Time
	Time 	10

	Event
		Ename	Hands01
		Type	Local
		At	WepR

		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
		Part	:MirrorSparkGlow.part
		Sound Magic_shieldup 69.0 69.0 .66
		Lifespan	55
	End

	Event
		Ename	Hands02
		Type	Local
		At	WepL
		
		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
		Part	:MirrorSparkGlow.part
		Lifespan	55
	End

End

###############  Mirror 01  #####################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Spin01
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		
		BhvrOverride
			PositionOffset	0 4.5 0
		End
				
	End

	Event
		Ename	Offset01
		Type	Local
		At	Spin01
		BhvrOverride
			Spin		0 0 0.075
			PositionOffset	0 0 2
		End
				
	End

	Event
		Ename	Case01
		Type	Local
		At	Offset01
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			#Spin		0 0 0.075
			SpinJitter	0.005 0.005 0.0055
			pyrRotate	10 10 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror01
		Type	Local
		At	Case01
		bhvr	:MirrorFade.bhvr
		
		Geom	FX_BronzeMirror	
		Sound magictoss1 70 70 .77
		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
	End

End

Condition
	On 	Time
	Time 	30

	Event
		Ename	Glow01
		Type	Local
		At	Case01
		
		BhvrOverride
			pyrRotate	-90 0 0
		End

		Part	:MirrorGlow_fadeFLAT.part
		
	End

End


###############  Mirror Project  #####################

Condition
	On 	Time
	Time 	65

	Event
		Ename	Launch
		Type	Start
		At	Case01
		
		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSparkGlow.part
		Part	:MirrorFlash.part
		Sound magictoss2 70 70 .77
		Lifespan	5
	End

	Event
		Ename	Prime
		Type	Start
		At	Case01
		bhvr	:MirrorProjectile.bhvr
		Geom	FX_BronzeMirror_Back
		Magnet	Target

		Lifespan	300
	End

	Event
		Ename	Mirror02
		Type	Local
		At	Prime
		
		Geom	FX_BronzeMirror	
		
		Part	:MirrorSmokeTrail.part	
		Part	:MirrorSmokeTrail_BIG.part
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
		Part	:MirrorSparkTrail_BIG.part
	End

	Event
		Ename	Glow02
		Type	Local
		At	Prime
		
		BhvrOverride
			pyrRotate	-90 0 0
		End

		Part	:MirrorGlow_FLAT.part
	End

	Event
		Ename	Spin01
		Type	Destroy
		
	End

End

###############  Mirror Hit  #####################


Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
		
	End

End


End
