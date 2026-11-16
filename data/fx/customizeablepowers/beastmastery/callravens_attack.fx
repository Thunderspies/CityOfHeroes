#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 20

		Event
			At Root
			Type Local
			Sound MurderOfCrows_01 100 100 0.8
		End

		Event
			At Root
			Type Local
			EName CrowAnchor
			BhvrOverride
				PositionOffset 0 4 -15
			End
			LifeSpan 45
		End

		Event
			At Root
			Type Start
			EName Offsets
			BhvrOverride
				Scale 30 30 30
			End
			Geom FX_ConeOffsets
		End

		Event
			AltPiv 1
			At Offsets
			Type Local
			EName CrowTarget
			BhvrOverride
				PositionOffset .1 0.25 0.1
				Scale .1 .1 .1
			End
		#	Geom GEO_FX_TestSphere
		End

	End

	Condition
		On Time
		Time 18

		Event
			At Origin
			Type Local
			ChildFx :CHILD_CallRavens_Attack.fx
			EName GeoCrows
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 30

		#Event
			#At Head
			#Type Start
			#EName Prime
			#LookAt CrowTarget
			#Magnet CrowTarget
			#Bhvr :CallSwarm_CC_Projectile.bhvr
		#End

		#Event
			#At Prime
			#Type Local
			#EName Trail
			#Part :CallRavens_Flock.part
		#End

		Event
			At CrowAnchor
			Type Start
			EName Prime
			PMagnet Head
			Part :CallRavens_Cone.part
			LifeSpan 25
		End

		Event
			At CrowAnchor
			Type Start
			EName Prime2
			PMagnet Head
			Part :CallRavens_Cone2.part
			Part :CallRavens_Cone3.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 37

		Event
			At Origin
			Type Local
			ChildFx :CHILD_CallRavens_Attack2.fx
			EName GeoCrows2
			LifeSpan 60
		End

	End

End
