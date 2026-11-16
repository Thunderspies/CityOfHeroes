#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName CrushingHateGeo
			#Bhvr :ForceBubble_Disintegration.bhvr
			BhvrOverride
				PositionOffset 0 2 0
				PyrRotateJitter 180 180 0
				Spin 0.05 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 255 50 255
			End
		End

		Event
			At CrushingHateGeo
			Type Local
			EName CrushingHateSwirls
			Flags OneAtATime
			#Bhvr :ForceBubble_Disintegration.bhvr
			BhvrOverride
				PyrRotateJitter -180 -180 0
				Spin 0.050 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 6 0 0
			End
			#Part :VoidOrb_Orb.part
			#Part :VoidOrb_Trail.part
			#Part :VoidOrb_Swirls.part
			#Part :VoidOrb_Swirls_Additive.part
			#Part :VoidOrb_Swirls_02.part
			Part ENEMYFX\Praetorian\MoM\test.part
			#Part :VoidOrb_Border.part
		End

	End

	#Condition
		#On Death

		#Event
			#Type Destroy
			#EName CrushingHateGeo
		#End

		#Event
			#Type Destroy
			#EName CrushingHateSwirls
		#End

		#Event
			#At Hips
			#Type PositOnly
			#EName Explosion
			#Part :VoidOrb_Streaks_Exploding.part
			#Part :VoidOrb_Streaks_Exploding_Additive.part
			#Part :VoidOrb_Swirls_Exploding.part
			#Part :VoidOrb_Border_Exploding.part
			#Part :VoidOrb_Smoke.part
			#Part :VoidOrb_Smoke_Lingering.part
			#LifeSpan 5
		#End

	#End

End
